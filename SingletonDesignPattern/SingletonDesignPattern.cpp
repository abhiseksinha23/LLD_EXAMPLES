// SINGLETON DESING PATTERN

#include "../../../builtin_files/bits-stdc++.h"
#include<bits/stdc++.h>
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
        if(instance == nullptr) {
            instance = new Singleton();
        }
        return instance;
    }
};

Singleton* Singleton::instance = nullptr;

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
