// WITHOUT MEDIATOR DESIGN PATTERN

/*
ISSUES -

Each User knows *all* the other directly (Tightly Coupled).
If you have N users, you wind up wiring N*(N-1)/2 connections,
and every new feature (mute, private send, logging, ...) lives in User too.
*/

#include <bits/stdc++.h>
using namespace std;


class User {
private:
	string name;
	unordered_map<string, shared_ptr<User>> peers;
	unordered_set<string>mutedUsers;

public:

	User(const string& name) {
		this->name = name;
	}

	string getName() {
		return name;
	}

	void addPeer(shared_ptr<User> u){
		peers[u->getName()] = u;
	}

	void mute(const string& userToMute) {
	   // cout <<name<< " " << userToMute << endl;
		mutedUsers.insert(userToMute);
// 		cout << mutedUsers.size() << endl;
// 		for(auto it : mutedUsers) {
// 		    cout <<it <<endl;
// 		}
	}

	// broadcast to all peers
	void send(const string &msg) {
		cout << "[" << name << " broadcases ]: " << msg << endl;
		for(auto it : peers) {
			auto peer = it.second;

			if(!isMuted(it.first)) {
			 //   cout <<" NOT MUTED " << peer->getName() <<endl;
				peer->receive(name, msg);
			} else {
			    cout <<"    Message not delivered to "<< peer->getName() << " as they have muted " << name << endl; 
			}
		}
	}

	bool isMuted(const string &name) {
	   // cout << (mutedUsers.find(name) != mutedUsers.end()) << endl;
		return (mutedUsers.find(name) != mutedUsers.end());
	}

	void sendTo(shared_ptr<User>target, const string &msg) {
		cout << "[" << name << " -> " << target->getName() << " ]:" << msg << endl;

		if(target->isMuted(name))return;

		target->receive(name, msg);
	}

	void receive(const string &from, const string &msg) {
		cout << "    " << name  << " got from " << from << ": " << msg << endl;
	}

};


int main () {

	shared_ptr<User> user1 = make_shared<User>("Rohan");
	shared_ptr<User> user2 = make_shared<User>("Neha");
	shared_ptr<User> user3 = make_shared<User>("Mohan");


	// wire up peers (each knows each other) -> n*(n-1)/2 connections
	user1->addPeer(user2);
	user1->addPeer(user3);

	user2->addPeer(user1);
	user2->addPeer(user3);

	user3->addPeer(user1);
	user3->addPeer(user2);

	// MUTE EXAMPLE : MOHAN MUTES ROHAN (HENCE ROHAN ADD MOHAN TO ITS MUTED LIST);
	user1->mute("Mohan");

	// BROADCAST
	user1->send("Hello everyone !!");


	// private
	user3->sendTo(user2, "Hey Neha!");	

	return 0;
}
