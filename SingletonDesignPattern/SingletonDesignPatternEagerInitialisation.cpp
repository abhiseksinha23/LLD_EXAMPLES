// SINGLETON DESING PATTERN EAGER INITIALIZATION
//
// NOT RECOMMENDED FOR MULTITHREADING ENVIRONMENTS AND HEAVY APPLICATIONS

#include "../../../builtin_files/bits-stdc++.h"
using namespace std;

class Singleton {
private:
    static Singleton *instance;

    // Private constructor to prevent instantiation
    Singleton() {
        cout << "Singleton instance created." << endl;
    }

public:
    static Singleton* getInstance() {
        return instance;
    }
};

Singleton* Singleton::instance = new Singleton(); // Eager initialization

int main() {
    cout << "Singleton Design Pattern Example" << endl;

    // Attempt to create multiple instances
    Singleton *singleton1 = Singleton::getInstance();
    Singleton *singleton2 = Singleton::getInstance();

    if(singleton1 == singleton2) {
        cout << "Both instances are the same." << endl;
    } else {
        cout << "Instances are different." << endl;
    }

    return 0;
}
