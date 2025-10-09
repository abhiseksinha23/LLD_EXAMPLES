// PROTOTYPE DESIGN PATTERN 
// EXAMPLE --> NPC (NON-PLAYING CHARACTERS)


#include <bits/stdc++.h>
using namespace std;


// Cloneable (aka Prototype) interface
class Cloneable {
public:
	virtual Cloneable* clone() const = 0;
};

class NPC : public Cloneable {
public:
	string name;
    int health;
    int attack;
    int defense;

    // "Heavy" constructor : DB connections, complex tasks, etc ...
	// This takes 5-15 seconds to create an object.

    NPC(const string& name, int health, int attack, int defense) {
        // call database
        // complex calc
        this->name = name; 
        this->health = health; 
        this->attack = attack; 
        this->defense = defense;
        cout << "Setting up template NPC '" << name << "'\n";
    }

     // copy‐ctor used by clone()
    NPC(const NPC& other) {
    	name = other.name;
        health = other.health;
        attack = other.attack;
        defense = other.defense;
        cout << "Cloning NPC '" << name << "'\n";
    }

    Cloneable* clone() const override {
    	return new NPC(*this);
    }

    void describe() {
        cout << "NPC " << name  << " [HP=" << health  << " ATK=" << attack 
             << " DEF=" << defense << "]\n";
    }

    // setters to tweak the clone…
    void setName(const string& n) { 
        name = n;
    }
    void setHealth(int h) { 
        health = h;
    }
    void setAttack(int a) {
         attack = a; 
    }
    void setDefense(int d){ 
        defense = d;
    }

};

int main() {
    // 1) build one “heavy” template
    NPC* alien = new NPC("Alien", 30, 5, 2);

    // 2) quickly clone + tweak as many variants as you like:
    NPC* alienCopied1 = dynamic_cast<NPC*>(alien->clone()); // Needs to Dynamic cast the return from Clonable to NPC (parent to child)
    alienCopied1->describe();

    NPC* alienCopied2 = dynamic_cast<NPC*>(alien->clone());
    alienCopied2->setName("Powerful Alien");
    alienCopied2->setHealth(50);
    alienCopied2->describe();

	// IF YOU WANT TO CREATE 100 ALIENS YOU DON'T HAVE TO DO THE HEAVY OPERATIONS 100 TIMES 
	// JUST COPY WILL WORK AND IT WILL SAVE COST (TIME).

    // cleanup
    delete alien;
    delete alienCopied1;
    delete alienCopied2;

    return 0;
}
