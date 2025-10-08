// MEDIATOR DESIGN PATTERN
// EXAMPLE --> CHATROOM

#include <bits/stdc++.h>
using namespace std;

// FORWARD DECLARATION
class IColleague;

class IMediator {
public:
	virtual void send(const string &from, const string &msg) = 0;
	virtual void sendPrivate(const string &from, const string &to, const string &msg) = 0;
	virtual void registerColleague(IColleague* c) = 0;
	//virtual void removeColleague(shared_ptr<IColleague> c) = 0;
};


class IColleague {
protected:
	IMediator* mediator;

public:
	IColleague(IMediator* m) {
		mediator = m;
	}

	virtual void sendPrivate(const string &to, const string &msg) = 0;
	virtual void send(const string &msg) = 0;
	virtual void receive(const string &from, const string &msg) = 0;
	virtual string getName() = 0;

};


class ChatMediator : public IMediator {
private:
	unordered_map<string, IColleague*>colleagues;
	unordered_map<string, unordered_set<string>>muted;
public:

	void registerColleague(IColleague* c) override {
		colleagues[c->getName()] = c;
	}

	void mute(const string &who, const string &whom) {
		muted[who].insert(whom);
	}


	// void unMute() {

	// }

	void send(const string &from, const string &msg) override {
		cout << "["<< from << " broadcasts]: "<<msg << endl;
		for(auto it : colleagues) {
			auto colleague = it.second;

			if(it.first == from) continue;

			if(muted[it.first].find(from) != muted[it.first].end()) {
				cout <<"	Cannot send msg because " << it.first <<" has muted " << from <<endl;
				continue;
			}

			colleague->receive(from, msg);
		}
	}
	

	void sendPrivate(const string &from, const string &to, const string &msg) override {
		cout << "[" << from << "->" << to << "]:" << msg << endl;

		if(muted[to].find(from) != muted[to].end()) {
			cout <<"	Cannot send msg because " << to <<" has muted " << from <<endl;
			return;
		}

		colleagues[to]->receive(from, msg);
	}
	
};


class User : public IColleague {
private:
	string name;

public:
	User(const string &n, IMediator* m) : name(n), IColleague(m) {
	    mediator->registerColleague(this);
	}

	string getName() {
		return name;
	}

	void sendPrivate(const string &to, const string &msg) {
		mediator->sendPrivate(name, to, msg);
	}

	void send(const string &msg) {
		mediator->send(name, msg);
	}

	void receive(const string &from, const string &msg) {
		cout <<"	" << name << " got msg from "<< from <<" : "<< msg <<endl;
	}
};



int main() {

	ChatMediator* chatRoom = new ChatMediator();

	User* user1 = new User("Rohan", chatRoom);
	User* user2 = new User("Neha", chatRoom);
	User* user3 = new User("Mohan", chatRoom);

	// MUTE EXAMPLE : MOHAN MUTES ROHAN (HENCE ROHAN ADD MOHAN TO ITS MUTED LIST);
	chatRoom->mute("Mohan" , "Rohan");

	// BROADCAST
	user1->send("Hello everyone !!");


	// private
	user3->sendPrivate("Neha", "Hey Neha!");	


	delete user3;
	delete user2;
	delete user1;
	delete chatRoom;

	return 0;
}
