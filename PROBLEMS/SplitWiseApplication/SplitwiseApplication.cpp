// LLD PROBLEM -->
// SPLITWISE
// SELF IMAGINED == NEED TO ADD DEBTSIMPLIFIER CODE

//////////////////////////////////////////////////////////////////
//          REQUIREMENTS 
//////////////////////////////////////////////////////////////////
/*

FUNCTIONAL REQUIREMENTS

- USER CAN JOIN/LEAVE A GROUP
- USER CAN ADD EXPENSE IN A GROUP
- USER CAN SETTLE AN EXPENSE IN A GROUP
- ADDING EXPENSE SHOULD BE BASED ON SEVERAL STRATEGIES
  (EQUAL SPLIT, %SPLIT, EXACT SPLIT)
- USER CAN'T LEAVE A GROUP WITHOUT SETTLING EXPENSES.
- USER CAN ALSO ADD INDIVIDUAL EXPENSES, OUTSIDE GROUP (1 ON 1)
- A NOTIFICATION TO BE SEND WHEN AN EXPENSE IS ADDED/ SETTLED.


NON-FUNCTIONAL REQUIREMENTS
- SCALABLE
- PLUG N PLAY
- FOLLOW SOLID PRINCIPLES
- LOOSLY COUPLED

DESIGN PATTERNS USED
- SINGLETON
- FACTORY
- STRATEGY
- FACADE
- OBSERVER


*/
//////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>
using namespace std;

// FORWARD DECLARTION
class User;
class NotificationSystem;


// -----------------------------------------------------------
//        NOTIFICATION SYSTEM
// -----------------------------------------------------------

class IObserver {
public:
  virtual void update(const string &message) = 0;
};

// -----------------------------------------------------------------------------------
//     USER
// -----------------------------------------------------------------------------------

class User : public IObserver {
private:
  static int nextUserId;
  string userId;
  string name;
  string email;

  unordered_map<string, double> balances;

public:

  User(string name, string email){
    userId = "user" + to_string(++nextUserId);
    this->name = name;
    this->email = email;
  }

  void update(const string& message) override {
        cout << "[NOTIFICATION to " << name << "]: " << message << endl;
  }

  string getId() {
    return userId;
  }

  string getName() {
    return name;
  }

  string getEmail() {
    return email;
  }

  void updateBalance(string otherId, double amount) {
      balances[otherId] += amount;

      if(balances[otherId] < 0.01 && balances[otherId] > -0.01){
        balances.erase(otherId);
      }
  }
  
  void showBalance() {
      cout<<"Balance sheet of " << name <<endl;
      if(balances.size() == 0){
           cout<<" All expenses settled up !!!" <<endl;
      }
      for(auto it : balances) {
          cout << it.first <<"  :  "  << it.second << endl;
      }
  } 
};

int User::nextUserId = 0;

// -----------------------------------------------------------------------------------
//     NOTIFICATION SYSTEM
// -----------------------------------------------------------------------------------

class NotificationSystem {
private:
  unordered_map<string, shared_ptr<User>> observers;
  static NotificationSystem *instance;
  static mutex mtx;

  NotificationSystem() {}

public:

  static NotificationSystem* getInstance() {
    if(instance == nullptr) {
      lock_guard<mutex> lock(mtx);
      if(instance == nullptr) {
        instance = new NotificationSystem();
      }
    }

    return instance;
  }

  void registerObserver(string userId, shared_ptr<User> user){
    observers[userId] = user;
  }

  void removeObserver(string userId){
    if(observers.find(userId) == observers.end())return;

    observers.erase(userId);
  }

  void notifyAll(const string msg) {
    for(auto it : observers) {
      it.second->update(msg);
    }
  }

  void notifyUser(string userId, const string msg){
    if(observers.find(userId) == observers.end())return;

    observers[userId]->update(msg);
  }
};


NotificationSystem* NotificationSystem::instance = nullptr;
mutex NotificationSystem::mtx;


// -----------------------------------------------------------------------------------
//     ONE TO ONE EXPENSE MANAGER
// -----------------------------------------------------------------------------------


class OneToOneExpenseManager {
private:
  static OneToOneExpenseManager* instance;
  static mutex mtx;

  OneToOneExpenseManager() {}

public:
  static OneToOneExpenseManager* getInstance() {
    if(instance == nullptr) {
      lock_guard<mutex> lock(mtx);
      if(instance == nullptr) {
        instance = new OneToOneExpenseManager();
      }
    }

    return instance;
  }

  void updateBalance(shared_ptr<User>u1, shared_ptr<User>u2, vector<double>amount) {
    u1->updateBalance(u2->getId(), amount[0]);
    u2->updateBalance(u1->getId(), amount[1]);

    string msg = u1->getName() +  " paid " +  to_string(amount[0]) + " amount to " + u2->getName();
    NotificationSystem::getInstance()->notifyUser(u1->getId(), msg);
    NotificationSystem::getInstance()->notifyUser(u2->getId(), msg);
  }
};

OneToOneExpenseManager* OneToOneExpenseManager::instance = nullptr;
mutex OneToOneExpenseManager::mtx;

// -----------------------------------------------------------------------------------
//     SPLIT STRATEGIES
// -----------------------------------------------------------------------------------


class Split {
public:
  string userId;
  double amount;

  Split(string id, double amt){
    userId = id;
    amount = amt;
  }
};


class SplitStrategy {
public:
  virtual vector<Split> calculateSplit(double amt, vector<string>users, vector<double>splitBy = {}) = 0;
};

class EqualSplitStrategy : public SplitStrategy {
public:

  vector<Split> calculateSplit(double amt, vector<string>users, vector<double>splitBy = {}) {
    vector<Split>splits;

    int n = users.size();
    double amount = amt/ (double)n;
    for(int i=0; i<n; i++){
      Split s1(users[i], amount);
      splits.push_back(s1);  
    }

    return splits;
  }
};


class PercentageSplitStrategy : public SplitStrategy {
public:

  vector<Split> calculateSplit(double amt, vector<string>users, vector<double>splitBy = {}) {
    vector<Split>splits;

    int n = users.size();
    
    for(int i=0; i<n; i++){
      double amount = (amt * splitBy[i])/ 100.0;
      Split s1(users[i], amount);
      splits.push_back(s1);  
    }

    return splits;
  }
};


class ExactSplitStrategy : public SplitStrategy {
public:

  vector<Split> calculateSplit(double amt, vector<string>users, vector<double>splitBy = {}) {
    vector<Split>splits;

    int n = users.size();
    for(int i=0; i<n; i++){
      Split s1(users[i], splitBy[i]);
      splits.push_back(s1);  
    }
    return splits;
  }
};


enum class SplitType {
EQUAL,
EXACT,
PERCENTAGE
};


class SplitFactory {
public:
  static shared_ptr<SplitStrategy> getSplitStrategy(SplitType type) {
    switch(type) {
    case SplitType::EQUAL : return make_shared<EqualSplitStrategy>();
    case SplitType::EXACT : return make_shared<ExactSplitStrategy>();
    case SplitType::PERCENTAGE : return make_shared<PercentageSplitStrategy>();
    default : return make_shared<EqualSplitStrategy>();
    }
  }
};


// -----------------------------------------------------------------------------------
//     EXPENSE
// -----------------------------------------------------------------------------------

class Expense {
public:
  string expenseId;
  static int nextExpenseId;
  string desc;

  string groupId;
  string paidUserId;

  vector<string>InvolvedUsers;
  double totalAmount;

  vector<Split>splits;

  Expense(string desc, string groupId, string paidUserId, vector<string>users, double amt, vector<Split>splits) {
    expenseId = "expense" + to_string(++nextExpenseId);
    this->desc = desc;
    this->groupId = groupId;
    this->paidUserId = paidUserId;
    this->InvolvedUsers = users;
    this->totalAmount = amt;
    this->splits = splits;
  }
};

int Expense::nextExpenseId = 0;


// -----------------------------------------------------------------------------------
//     DEBT SIMPLIFIER
// -----------------------------------------------------------------------------------

class DebtSimplifier {

};


// -----------------------------------------------------------------------------------
//     GROUP
// -----------------------------------------------------------------------------------

class Group {
private:  
  string groupId;
  static int nextGroupId;

  string name;
  unordered_map<string, shared_ptr<User>>users;
  unordered_map<string, Expense> expenses;
  unordered_map<string, unordered_map<string, double >> balances;

public:

  Group(string name) {
    this->name = name;
    groupId = "group" + to_string(++nextGroupId);
  }

  void addMember(shared_ptr<User> u) {
    users[u->getId()] = u;
    balances[u->getId()] = unordered_map<string, double>();
  }

  void removeMember(string id) {
    if(users.find(id) == users.end())return;

    users.erase(id);
    balances.erase(id);
  }

  void notifyGroupMembers(string msg) {
    for(auto user : users) {
      NotificationSystem::getInstance()->notifyUser(user.first, msg);
    }
  }

  void updateBalance(string u1, string u2, double amt) {
    balances[u1][u2] += amt;
    // cout << balances[u1][u2] <<endl;
    users[u1]->updateBalance(u2, amt);

    if(balances[u1][u2] > -0.01 && balances[u1][u2] < 0.01 ) {
      balances[u1].erase(u2);
    }
    
    // cout << balances[u1][u2] <<endl;
  }

  void addExpense(string desc, string paidUserId, vector<string>usersId, double amt, vector<Split>splits) {
    string msg = (users[paidUserId])->getName() + " paid " + to_string(amt) + " amount";
    notifyGroupMembers(msg);

    for(auto split : splits) {  
      updateBalance(paidUserId, split.userId, split.amount);
      updateBalance(split.userId, paidUserId, -split.amount);
    }

    Expense expense( desc, groupId, paidUserId, usersId, amt, splits);
    // expenses[expense.expenseId] = expense;
  }

  void settleBalance(string u1, string u2, double amt) {
    string msg = users[u1]->getName() + " paid " + to_string(amt) + " amount to " + users[u2]->getName();
    notifyGroupMembers(msg);
    updateBalance(u1,u2, amt);
    updateBalance(u2,u1, -amt);
  }
  
  void showBalance() {
      for(auto it : balances){
          cout<<"Balance sheet for User : " << users[it.first]->getName() <<" : " <<endl;
          if(it.second.size() == 0){
              cout<<" All expenses settled up inside the group!!!" <<endl;
          }
          for(auto it2: it.second){
              cout << users[it2.first]->getName() <<" : " << it2.second << endl;
          }
          cout<<endl;
      }
  }


};

int Group::nextGroupId = 0;

// -----------------------------------------------------------------------------------
//     SPLITWISE FACADE
// -----------------------------------------------------------------------------------


int main() {

  OneToOneExpenseManager *oneManager = OneToOneExpenseManager::getInstance();

  shared_ptr<User> u1 = make_shared<User>("ABHISEK", "ABHISEK@GMAIL.COM");
  shared_ptr<User> u2 = make_shared<User>("GAURAV", "GAURAV@GMAIL.COM");

  NotificationSystem::getInstance()->registerObserver(u1->getId(), u1);
  NotificationSystem::getInstance()->registerObserver(u2->getId(), u2);

  oneManager->updateBalance(u1, u2, {20.0, -20.0});
  
  shared_ptr<SplitStrategy> strat = SplitFactory::getSplitStrategy(SplitType::EXACT);

  auto vec = strat->calculateSplit(500.0, {u1->getId(), u2->getId()}, {300, 200});

  for(auto v : vec) {
    cout<< v.userId <<" "<< v.amount << endl;
  }

  u1->showBalance();
  u2->showBalance();
  
  Group G1("Hostel");
  G1.addMember(u1);
  G1.addMember(u2);
  
  G1.addExpense("lunch", u1->getId(), {u2->getId()}, 500, vec);
  G1.showBalance();
  u1->showBalance();
  u2->showBalance();
  G1.settleBalance(u2->getId(), u1->getId(), 200);
  u1->showBalance();
  u2->showBalance();
  
  G1.showBalance();
  
  oneManager->updateBalance(u1, u2, {-20.0, +20.0});
  u1->showBalance();
  u2->showBalance();
  return 0;
}


