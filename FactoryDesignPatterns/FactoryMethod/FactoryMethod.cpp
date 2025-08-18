// FACTORY METHOD DESING PATTERN EXAMPLE --> BURGER FACTORY

#include "../../../../builtin_files/bits-stdc++.h"
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

class BasicWheatBurger: public Burger {
public:
    void prepare() override {
        cout << "Preparing Basic Wheat Burger with wheat bun, patty, and lettuce." << endl;
    }
};

class StandardWheatBurger : public Burger {
public:
    void prepare() override {
        cout << "Preparing Standard Wheat Burger with wheat bun, patty, lettuce, tomato, and cheese." << endl;
    }
};

class PremiumWheatBurger : public Burger {
public:
    void prepare() override {
        cout << "Preparing Premium Wheat Burger with wheat bun, patty, lettuce, tomato, cheese, bacon, and special sauce." << endl;
    }
};

class BurgerFactory {
public:
    virtual Burger* createBurger(string &type) = 0;
};

class SinghBurgers: public BurgerFactory {
public:
    Burger* createBurger(string &type) override {
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

class KingBurgers: public BurgerFactory {
public:
    Burger* createBurger(string &type) override {
        if(type == "basic") {
            return new BasicWheatBurger();
        } else if(type == "standard") {
            return new StandardWheatBurger();
        } else if(type == "premium") {
            return new PremiumWheatBurger();
        } else {
            cout << "Invalid burger type!" << endl;
            return nullptr;
        }
    }
};

int main(){
    cout<<"Factory Method Design Pattern Example\n";
    BurgerFactory *singhFactory = new SinghBurgers();
    BurgerFactory *kingFactory = new KingBurgers();

    string type;
    cout<<"Now switching to Singh Burgers...\n";

    type = "basic";
    Burger* burger = singhFactory->createBurger(type);
    burger->prepare();

    type = "standard";
    burger = singhFactory->createBurger(type);
    burger->prepare();

    type = "premium";
    burger = singhFactory->createBurger(type);
    burger->prepare();

    cout<<"Now switching to King Burgers...\n";

    type = "basic";
    burger = kingFactory->createBurger(type);
    burger->prepare();

    type = "standard";
    burger = kingFactory->createBurger(type);
    burger->prepare();

    type = "premium";
    burger = kingFactory->createBurger(type);
    burger->prepare();

    delete singhFactory; // Clean up memory
    delete kingFactory; // Clean up memory
    delete burger; // Clean up memory
    return 0;
}
