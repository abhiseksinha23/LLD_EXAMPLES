// ABSTRACT FACTORY METHOD DESING PATTERN EXAMPLE --> BURGER FACTORY

#include "../../../../builtin_files/bits-stdc++.h"
using namespace std;

// PRODUCT ONE - BURGER
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

// PRPODUCT 2 - GARLIC BREAD

class GarlicBread {
public:
    virtual void prepare() = 0; // Pure virtual function
};

class BasicGarlicBread: public GarlicBread {
public:
    void prepare() override {
        cout << "Preparing Basic Garlic Bread with butter and garlic." << endl;
    }
};

class CheeseGarlicBread : public GarlicBread {
public:
    void prepare() override {
        cout << "Preparing Cheese Garlic Bread with butter, garlic, and cheese." << endl;
    }
};

class BasicWheatGarlicBread: public GarlicBread {
public:
    void prepare() override {
        cout << "Preparing Basic Wheat Garlic Bread with wheat bread, butter, and garlic." << endl;
    }
};

class CheeseWheatGarlicBread : public GarlicBread {
public:
    void prepare() override {
        cout << "Preparing Cheese Wheat Garlic Bread with wheat bread, butter, garlic, and cheese." << endl;
    }
};

/// ////////////////////////////////////////////

class MealFactory {
public:
    virtual Burger* createBurger(string &type) = 0;
    virtual GarlicBread* createGarlicBread(string &type) = 0;
};

class SinghBurgers: public MealFactory {
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

    GarlicBread* createGarlicBread(string &type) override {
        if(type == "basic") {
            return new BasicGarlicBread();
        } else if(type == "cheese") {
            return new CheeseGarlicBread();
        } else {
            cout << "Invalid garlic bread type!" << endl;
            return nullptr;
        }
    }
};

class KingBurgers: public MealFactory {
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

    GarlicBread* createGarlicBread(string &type) override {
        if(type == "basic") {
            return new BasicWheatGarlicBread();
        } else if(type == "cheese") {
            return new CheeseWheatGarlicBread();
        } else {
            cout << "Invalid garlic bread type!" << endl;
            return nullptr;
        }
    }
};

int main(){
    cout<<"Factory Method Design Pattern Example\n";
    MealFactory *singhFactory = new SinghBurgers();
    MealFactory *kingFactory = new KingBurgers();

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

    type = "basic";
    GarlicBread* garlicBread = singhFactory->createGarlicBread(type);
    garlicBread->prepare();

    type = "cheese";
    garlicBread = singhFactory->createGarlicBread(type);
    garlicBread->prepare();

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

    type = "basic";
    garlicBread = kingFactory->createGarlicBread(type);
    garlicBread->prepare();

    type = "cheese";
    garlicBread = kingFactory->createGarlicBread(type);
    garlicBread->prepare();

    delete singhFactory; // Clean up memory
    delete kingFactory; // Clean up memory
    delete burger; // Clean up memory
    delete garlicBread; // Clean up memory
    return 0;
}
