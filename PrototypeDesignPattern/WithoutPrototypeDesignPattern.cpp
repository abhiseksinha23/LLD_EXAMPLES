// WITHOUT PROTOTYPE DESIGN PATTERN 
// EXAMPLE --> NPC (NON-PLAYING CHARACTERS)


#include <bits/stdc++.h>
using namespace std;


// SINGLE NPC CLASS - NO PROTOTYPE
class NPC {
public:
	string name;
	int health;
	int attack;
	int defense;

	// "Heavy" constructor : DB connections, complex tasks, etc ...
	// This takes 5-15 seconds to create an object.

	NPC(const string& name, int health, int attack, int defense) {
		this->name = name;
		this->health = health;
		this->attack = attack;
		this->defense = defense;

		cout << "Creating NPC '" << name <<"' [HP:" <<health << ", ATK :"
			 << attack << ", DEF:" << defense <<"]" << endl; 
	}

	void describe() {
		cout << "  NPC: " << name << " | HP=" << health << " ATK=" << attack
             << " DEF=" << defense << "\n";
	}
};

int main() {

	//Base Alien
	shared_ptr<NPC> alien = make_shared<NPC>("Alien", 30, 5, 2);
	alien->describe();


	//Powerful Alien
	shared_ptr<NPC> alien2 = make_shared<NPC>("Alien", 30, 5, 5);
	alien2->describe();

	return 0;
}