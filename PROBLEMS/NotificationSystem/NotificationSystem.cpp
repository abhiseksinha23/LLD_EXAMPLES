/*
NOTIFICATION SERVICE EXAMPLE

Requirements -
1. It's should be a plug and play service.
2.1. Extensible : SMS, EMAIL, Pop-UP,
2.2. logger
3. Notifcation can be modified dynamicall.
4. Store all Notifications.

*/


#include <bits/stdc++.h>
//#include "../../../builtin_files/bits-stdc++.h"
using namespace std;

/////////////////////////////////////////////////
//           INOTIFICATION && DECORATORS
////////////////////////////////////////////////

class INotification {
public:
	virtual string getContent() = 0;
	virtual ~INotification() {};
};

class SimpleNotification : public INotification {
private:
	string content;
public:
	SimpleNotification(string content) {
		this->content = content;
	}

	string getContent() override {
		return content;
	}
};

class INotificationDecorator : public INotification {
protected:
	INotification *notification;
};

class TimeStampDecorator : public INotificationDecorator {
public:
	TimeStampDecorator(INotification *notification) {
		this->notification = notification;
	}

	string getContent() override {
		return "[24-08-25 19:31:56] " + notification->getContent() + "\n";
	}
};

class SignatureDecorator : public INotificationDecorator {
private:
	string signature;
public:
	SignatureDecorator(INotification *notification, string sign){
		this->notification = notification;
		this->signature = sign;
	}

	string getContent() override {
		return notification->getContent() + "-- "  + signature + " \n";
	}
};

/////////////////////////////////////////////////
//           IOSERVERABLE && IOBSERVERS
////////////////////////////////////////////////

class IObserver{
public:
	virtual void update() = 0;
};

class IObservable {
public:
	virtual void addObserver(IObserver * observer) = 0;
	virtual void removeObserver(IObserver * observer) = 0;
	virtual void notifyObservers() = 0;
};

class Observable : public IObservable {
private:
	INotification *notification;
	set<IObserver*>observers;
public:
	Observable(){
		notification = nullptr;
	}
	void addObserver(IObserver * observer) override {
		observers.insert(observer);
	}
	void removeObserver(IObserver * observer) override {
		observers.erase(observer);
	}
	void notifyObservers() override {
		for(auto obs: observers){
			obs->update();
		}
	}

	INotification* getNotification() {
		return notification;
	}

	string getNotificationMessage() {
		return notification->getContent();
	}

	void SetNotification(INotification *notification){
		this->notification = notification;
		notifyObservers();
	}
};



/////////////////////////////////////////////////
//           NOTIFICATION SERVICE
////////////////////////////////////////////////

// SINGLETON
class NotificationService {
private:
	vector<INotification*>notifications;
	Observable *observable;

	static NotificationService *instance;
	static mutex mtx;

	NotificationService(){
		this->observable = new Observable();
	}
public:

	static NotificationService* getInstance(){
		if(instance == nullptr){
			lock_guard<mutex> lock(mtx);
			if(instance == nullptr){
				instance = new NotificationService();
			}
		}
		return instance;
	}

	void addNotification(INotification *notification){
		notifications.push_back(notification);
		observable->SetNotification(notification);
	}

	Observable* getObservable(){
		return observable;
	}
};

NotificationService* NotificationService::instance = nullptr;
mutex NotificationService::mtx;

/////////////////////////////////////////////////
//           NOTIFICATION STRATEGY
////////////////////////////////////////////////

class INotificationStrategy {
public:
	virtual void sendNotification(string content) = 0;
};

class SMSStrategy : public INotificationStrategy {
private:
	string number;
public:
	SMSStrategy(string number){
		this->number = number;
	}

	void sendNotification(string content){
		cout<<" Sending notification via SMS to the number: "<< number << endl << content << endl;
	}
};

class EmailStrategy : public INotificationStrategy {
private:
	string email;
public:
	EmailStrategy(string email){
		this->email = email;
	}

	void sendNotification(string content){
		cout<<" Sending notification to email : "<< email << endl << content << endl;
	}
};

class PopUpStrategy : public INotificationStrategy {
public:
	void sendNotification(string content){
		cout<<" Sending notification via POP-UP : \n"<< content << endl;
	}
};


/////////////////////////////////////////////////
//           OBSERVERS LOGGER
////////////////////////////////////////////////

class Logger : public IObserver {
private:
	Observable *observable;
public:
	Logger(){
		this->observable = NotificationService::getInstance()->getObservable();
		observable->addObserver(this);
	}

	Logger(Observable *observable) {
		this->observable = observable;
	}

	void update(){
		cout<<"LOGGING THE MESSAGE IN THE LOG FILE ---> "<< observable->getNotificationMessage() << endl;
	}
};


/////////////////////////////////////////////////
//           OBSERVERS NOTIFICATION ENGINE
////////////////////////////////////////////////

class NotificationEngine : public IObserver {
private:
	Observable *observable;
	vector<INotificationStrategy*> notificationStrategies;
public:
	NotificationEngine(){
		this->observable = NotificationService::getInstance()->getObservable();
		observable->addObserver(this);
	}
	NotificationEngine(Observable *observable) {
		this->observable = observable;
	}

	void addNotificationStrategy(INotificationStrategy *ns){
		notificationStrategies.push_back(ns);
	}

	void update(){
		for(auto ns : notificationStrategies){
			ns->sendNotification(observable->getNotificationMessage());
		}
	}
};

/////////////////////////////////////////////////
//           CLIENT CODE
////////////////////////////////////////////////

int main() {
	NotificationService *notificationService = NotificationService::getInstance();

	Logger *logger = new Logger();

	NotificationEngine *notificationEngine = new NotificationEngine();
	notificationEngine->addNotificationStrategy(new EmailStrategy("abc.xyz@gmail.com"));
	notificationEngine->addNotificationStrategy(new SMSStrategy("9876543210"));
	notificationEngine->addNotificationStrategy(new PopUpStrategy());

	INotification* notification = new SimpleNotification("Your Order has been shipped !!!");
	notification  = new TimeStampDecorator(notification);
	notification  = new SignatureDecorator(notification, "Logistic Team");

	notificationService->addNotification(notification);	

	delete notification;
	delete logger;
	delete notificationEngine;

	return 0;
}
