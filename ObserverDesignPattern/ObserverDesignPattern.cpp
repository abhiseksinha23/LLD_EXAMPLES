// OBSERVER DESING PATTERN
#include<bits/stdc++.h>
//#include "../../../builtin_files/bits-stdc++.h"
using namespace std;

////////////////// ISubscriber /////////////////////

class ISubscriber {
public:
	virtual void update() = 0;
};

////////////////// ICHANNEL /////////////////////

class IChannel {
	virtual void addSubscriber(ISubscriber* subscriber) = 0;
	virtual void removeSubscriber(ISubscriber* subscriber) = 0;
	virtual void notifySubscriber() = 0;
};

////////////////// CHANNEL /////////////////////

class Channel : public IChannel {
private:
	string name;
	string latestVideo;
	set<ISubscriber*> subscribers;

public:

	Channel(string name){
		this->name = name;
	}

	string getVideo(){
		return "\nCheckout the new video : " + latestVideo + "\n";
	}

	void addSubscriber(ISubscriber* subscriber) override {
		subscribers.insert(subscriber);
	}

	void removeSubscriber(ISubscriber* subscriber) override {
		subscribers.erase(subscriber);
	}

	void notifySubscriber() override {
		for( auto  subs:  subscribers){
			subs->update();
		}
	}

	void uploadVideo(string title) {
		latestVideo = title;
		cout<<"\n[" << name <<" uploaded \"" << title << "\"]" << endl;
		notifySubscriber();
	}
};

////////////////// Subscriber /////////////////////

class Subscriber : public ISubscriber{
private:
	string name;
	Channel *channel;
public:
	Subscriber(string name, Channel *channel){
		this->name = name;
		this->channel = channel;
	}

	void update() override {
		cout<<" Hey "<< name <<"," << this->channel->getVideo();
	}
};


////////////////////// MAIN ////////////////////////////

int main(){
	Channel *channel = new Channel("KEEP-ROCKING!!!");

	Subscriber *sub1 = new Subscriber("Abhisek", channel);
	Subscriber *sub2 = new Subscriber("Gaurav", channel);

	// USER SUBSCRIBES TO CHANNEL
	channel->addSubscriber(sub1);
	channel->addSubscriber(sub2);


	channel->uploadVideo("Learning about Observers Pattern");

	channel->removeSubscriber(sub2);

	channel->uploadVideo("Learning about Observers Pattern another video !!");

	return 0;
}

