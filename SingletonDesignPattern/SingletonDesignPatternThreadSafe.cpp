// SINGLETON DESING PATTERN WITH THREAD SAFETY

#include "../../../builtin_files/bits-stdc++.h"
using namespace std;

class Singleton {
private:
    static Singleton *instance;
    static mutex mtx; // Mutex for thread safety

    // Private constructor to prevent instantiation
    Singleton() {
        cout << "Singleton instance created." << endl;
    }

public:
    static Singleton* getInstance() {
        if(instance == nullptr) {
            lock_guard<mutex> lock(mtx); // Lock the mutex to ensure thread safety
            if(instance == nullptr) { // Double-checked locking
              instance = new Singleton();
            }
        }
        return instance;
    }
};

Singleton* Singleton::instance = nullptr;
mutex Singleton::mtx; // Initialize the mutex

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
