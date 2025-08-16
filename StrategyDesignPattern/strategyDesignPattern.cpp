//#include "../../../builtin_files/bits-stdc++.h"
#include<bits/stdc++.h>
using namespace std;
// Strategy Design Pattern Example

// Talkable interface
class Talkable{
public:
    virtual void talk() = 0; // Pure virtual function
};

class NormalTalk : public Talkable{
public:
    void talk() override{
        cout << "Talking normally." << endl;
    }
};

class NoTalk : public Talkable{
public:
    void talk() override{
        cout << "Can't talk !!!" << endl;
    }
};

// Walkable interface
class Walkable{
public:
    virtual void walk() = 0; // Pure virtual function
};

class NormalWalk :public Walkable{
public:
    void walk() override{
        cout << "Walking normally." << endl;
    }
};

class NoWalk : public Walkable{
public:
    void walk() override{
        cout << "Can't walk !!!" << endl;
    }
};

// 
class Flyable{
public:
    virtual void fly() = 0; // Pure virtual function
};

class NormalFly : public Flyable{
public:
    void fly() override{
        cout << "Flying normally." << endl;
    }
};

class NoFly : public Flyable{
public:
    void fly() override{
        cout << "Can't fly !!!" << endl;
    }
};

// Client class
class Robot{
private:
    Talkable *t;
    Walkable *w;
    Flyable *f;
public:
    Robot(Talkable *t, Walkable *w, Flyable *f){
        this->t = t;
        this->w = w;
        this->f = f;
    }

    void talk(){
        t->talk();
    }

    void walk(){
        w->walk();
    }

    void fly(){
        f->fly();
    }

    virtual void projection() = 0;
};

class CompanionRobot : public Robot{
public:
    CompanionRobot(Talkable *t, Walkable *w, Flyable *f) : Robot(t, w, f) {}

    void projection() override {
        cout << "Companion Robot is projecting." << endl;
    }
};

class WorkerRobot : public Robot{
public:
    WorkerRobot(Talkable *t, Walkable *w, Flyable *f) : Robot(t, w, f) {}

    void projection() override {
        cout << "Worker Robot is projecting." << endl;
    }
};

int main(){
    // Create instances of behaviors
    Talkable *normalTalk = new NormalTalk();
    Walkable *normalWalk = new NormalWalk();
    Flyable *normalFly = new NormalFly();

    Talkable *noTalk = new NoTalk();
    Walkable *noWalk = new NoWalk();
    Flyable *noFly = new NoFly();

    // Create Companion Robot with normal behaviors
    Robot *companionRobot = new CompanionRobot(normalTalk, normalWalk, normalFly);
    companionRobot->talk();
    companionRobot->walk();
    companionRobot->fly();
    companionRobot->projection();

    // Create Worker Robot with no talk behavior
    Robot *workerRobot = new WorkerRobot(noTalk, normalWalk, noFly);
    workerRobot->talk();
    workerRobot->walk();
    workerRobot->fly();
    workerRobot->projection();

    // Clean up
    delete normalTalk;
    delete normalWalk;
    delete normalFly;
    delete noTalk;
    delete noWalk;
    delete noFly;
    delete companionRobot;
    delete workerRobot;

    return 0;
}
