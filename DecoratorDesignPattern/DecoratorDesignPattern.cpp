// DECORATOR DESING PATTERN

//#include "../../../builtin_files/bits-stdc++.h"
#include<bits/stdc++.h>
using namespace std;

////////////////// ICHARACTER /////////////////////

class ICharacter {
public:
	virtual string getAbilities() = 0;
	virtual ~ICharacter() {} // virtual Destructor
};

//////////////// MARIO CHARACTER //////////////////

class Mario : public ICharacter {
public:
	string getAbilities() override {
		return  "Normal Mario Character";
	}
};


////////////////// IDECORATOR /////////////////////

class IDecorator : public ICharacter{
protected:
	ICharacter *character;
public:
	IDecorator(ICharacter *character){
		this->character = character;
	}
};


////////////////// HEIGHTUPDECORATOR /////////////////////

class HeightUpDecorator : public IDecorator{
public:
	HeightUpDecorator(ICharacter *character) : IDecorator(character){

	}

	string getAbilities() override {
		return this->character->getAbilities() + " with Height Up";
	}
};

////////////////// STARPOWERDECORATOR /////////////////////

class StarPowerDecorator : public IDecorator{
public:
	StarPowerDecorator(ICharacter *character) : IDecorator(character){

	}

	string getAbilities() override {
		return this->character->getAbilities() + " with Star Power for a limited time";
	}
};


////////////////// GUNPOWERDECORATOR /////////////////////

class GunPowerDecorator : public IDecorator{
public:
	GunPowerDecorator(ICharacter *character) : IDecorator(character){

	}

	string getAbilities() override {
		return this->character->getAbilities() + " with Gun Power";
	}
};


int main(){

	ICharacter *mario = new Mario();
	cout<< mario->getAbilities() << endl;

	mario = new HeightUpDecorator(mario);
	cout<< mario->getAbilities() << endl;


	mario = new GunPowerDecorator(mario);
	cout<< mario->getAbilities() << endl;

	mario = new StarPowerDecorator(mario);
	cout<< mario->getAbilities() << endl;

	delete mario;

	return 0;
}

