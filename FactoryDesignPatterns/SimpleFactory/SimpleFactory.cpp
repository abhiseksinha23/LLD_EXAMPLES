// SIMPLE FACTORY (DESING PRINCIPLE NOT A PATTERN) EXAMPLE --> BURGER FACTORY

// #include "../../../../builtin_files/bits-stdc++.h"
#include<bits/stdc++.h>
using namespace std;

class Burger {
public:
    virtual void prepare() = 0; // Pure virtual function
};

class BasicBurger: public Burger {
public:
    void prepare() override {
        cout << "Preparing Basic Burger with bun, patty, and lettuce." << endl;
    }
};

class StandardBurger : public Burger {
public:
    void prepare() override {
        cout << "Preparing Standard Burger with bun, patty, lettuce, tomato, and cheese." << endl;
    }
};

class PremiumBurger : public Burger {
public:
    void prepare() override {
        cout << "Preparing Premium Burger with bun, patty, lettuce, tomato, cheese, bacon, and special sauce." << endl;
    }
};

class BurgerFactory {
public:
    Burger* createBurger(string &type){
        if(type == "basic") {
            return new BasicBurger();
        } else if(type == "standard") {
            return new StandardBurger();
        } else if(type == "premium") {
            return new PremiumBurger();
        } else {
            cout << "Invalid burger type!" << endl;
            return nullptr;
        }
    }
};

int main(){
    cout<<"Simple Factory Design Principle Example\n";
    BurgerFactory factory;
    string type;
    type = "basic";
    Burger* burger = factory.createBurger(type);
    burger->prepare();
    
    type = "standard";
    burger = factory.createBurger(type);
    burger->prepare();

    type = "premium";
    burger = factory.createBurger(type);
    burger->prepare();

    delete burger; // Clean up memory
    return 0;
}
