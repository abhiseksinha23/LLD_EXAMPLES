// PROXY DESIGN PATTERN --> PROTECTION PROXY
// EXAMPLE --> DOC READER

#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////////////////
// 					IDOCREADER 
//////////////////////////////////////////////////////////////////

class IDocReader {
public:
	virtual void unlockPDF(const string &file, const string &pwd) = 0;
	virtual ~IDocReader() {};
};


//////////////////////////////////////////////////////////////////
// 					REALDOCREADER 
//////////////////////////////////////////////////////////////////

class RealDocReader : public IDocReader {
public:
	void unlockPDF(const string &file, const string &pwd) override {
		cout<<"[REALDOCREADER] Unlocking the Doc : " << file
		<< " using the password : "<<pwd
		<< endl;

		cout<<"[REALDOCREADER] unlocked successfully !!!"<<endl;
	}
};

//////////////////////////////////////////////////////////////////
// 					USER 
//////////////////////////////////////////////////////////////////

class User {
private:
	string name;
	bool isPremium;

public:
	User(const string &name, bool isPremium){
		this->name = name;
		this->isPremium = isPremium;
	}

	string getName(){
		return name;
	}

	bool isPremiumUser(){
		return isPremium;
	}
};


//////////////////////////////////////////////////////////////////
// 					PROXY 
//////////////////////////////////////////////////////////////////

class Proxy : public IDocReader {
private:
	shared_ptr<RealDocReader> rd;
	shared_ptr<User> user;

public:
	Proxy(shared_ptr<RealDocReader> rd, shared_ptr<User>user){
		this->rd = rd;
		this->user = user;
	}

	void unlockPDF(const string &file, const string &pwd) override {
		if(user->isPremiumUser() == false){
			cout<<"[PROXY] "<<  user->getName() <<" is not a premium user "<< endl;
			cout<<"[PROXY] feature available only for premium users !!!" <<endl;
			return;
		}
		cout<<"[PROXY] Premium user  : " << user->getName() <<endl;
		rd->unlockPDF(file, pwd);
	}
}; 

//////////////////////////////////////////////////////////////////
// 					CLIENT 
//////////////////////////////////////////////////////////////////
int main(){
	shared_ptr<RealDocReader> realDocReader = make_shared<RealDocReader>();

	shared_ptr<User> user1 = make_shared<User>("Abhisek", true);
	unique_ptr<IDocReader> reader1 = make_unique<Proxy>(realDocReader, user1);

	shared_ptr<User> user2 = make_shared<User>("Gaurav", false);
	unique_ptr<IDocReader> reader2 = make_unique<Proxy>(realDocReader, user2);

	reader1->unlockPDF("/c:/docs/notes.pdf", "1234");
	reader2->unlockPDF("/c:/docs/notes.pdf", "1234");

	return 0;
}

