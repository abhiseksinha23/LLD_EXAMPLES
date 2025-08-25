// COMMAND DESING PATTERN EXAMPLE --> REMOTE CONTROLLED APPLICANCE

//#include "../../../builtin_files/bits-stdc++.h"
#include<bits/stdc++.h>
using namespace std;

////////////////// ICOMMAND /////////////////////

class ICommand {
public:
	virtual void execute() = 0;
	virtual void undo() = 0;
	virtual ~ICommand() {}
};


////////////////// REMOTE CONTROL /////////////////

class RemoteControl {
private:
	static const int buttons = 4;
	vector<ICommand*>commands;
	vector<bool>isPressed;
public:
	RemoteControl() {
		commands.resize(buttons);
		isPressed.resize(buttons);
		for(int i=0; i<buttons; i++){
			commands[i] = nullptr;
			isPressed[i] = false;
		}
	}

	void setCommand(ICommand *com, int index){
		if(index < 0 || index >= buttons){
			cout<<"No a valid button index"<<endl;
			return;
		}
		if(commands[index] != nullptr){
			delete commands[index];
		}

		commands[index] = com;
		isPressed[index] = false;
	}

	void pressButtom(int index){
		if(index < 0 || index >= buttons || commands[index] == nullptr){
			cout<<"No Command assigned to this button"<<endl;
			return;
		}
		if(isPressed[index]){
			commands[index]->undo();
		}else{
			commands[index]->execute();
		}
		isPressed[index] = !isPressed[index];
	}

	~RemoteControl(){
		for(int i=0; i<buttons; i++){
			if(commands[i] != nullptr){
				delete commands[i];
			}
		}
	}
};

////////////////// LIGHT /////////////////
class Light {
public:
	void on(){
		cout<<"Light turned on" <<endl;
	}
	void off(){
		cout<<"Light turned off" <<endl;
	}
};

////////////////// FAN /////////////////
class Fan {
public:
	void on(){
		cout<<"Fan turned on" <<endl;
	}
	void off(){
		cout<<"Fan turned off" <<endl;
	}
};

////////////////// AC /////////////////
class AC {
public:
	void on(){
		cout<<"AC turned on" <<endl;
	}
	void off(){
		cout<<"AC turned off" <<endl;
	}
};

////////////////// LIGHT COMMAND /////////////////
class LightCommand : public ICommand{
	Light *light;
public:
	LightCommand(Light *light) {
		this->light = light;
	}
	void execute() {
		light->on();
	}
	void undo() {
		light->off();
	}
};
////////////////// FAN COMMAND /////////////////

class FanCommand : public ICommand{
	Fan *fan;
public:
	FanCommand(Fan *fan) {
		this->fan = fan;
	}
	void execute() {
		fan->on();
	}
	void undo() {
		fan->off();
	}
};

////////////////// AC COMMAND /////////////////

class ACCommand : public ICommand{
	AC *ac;
public:
	ACCommand(AC *ac) {
		this->ac = ac;
	}
	void execute() {
		ac->on();
	}
	void undo() {
		ac->off();
	}
};


////////////////// CLIENT CODE /////////////////

int main()
{
	RemoteControl *remoteControl = new RemoteControl();

	Light *light = new Light();
	Fan *fan = new Fan();
	AC *ac = new AC();

	ICommand *lightCommand = new LightCommand(light);
	ICommand *fanCommand = new FanCommand(fan);
	ICommand *acCommand = new ACCommand(ac);

	remoteControl->setCommand(lightCommand, 0);
	remoteControl->setCommand(fanCommand, 1);
	remoteControl->setCommand(acCommand, 2);

	cout<<"==============================="<<endl;
	// TOGGLE THE LIGHT ON AND OFF
	remoteControl->pressButtom(0);
	remoteControl->pressButtom(0);
	cout<<endl;	

	cout<<"==============================="<<endl;
	// TOGGLE THE FAN ON AND OFF
	remoteControl->pressButtom(1);
	remoteControl->pressButtom(1);
	cout<<endl;

	cout<<"==============================="<<endl;
	// TOGGLE THE AC ON AND OFF
	remoteControl->pressButtom(2);
	remoteControl->pressButtom(2);
	cout<<endl;

	cout<<"==============================="<<endl;
	// NOT ASSIGNED BUTTON
	remoteControl->pressButtom(3);
	cout<<endl;

	return 0;
}

