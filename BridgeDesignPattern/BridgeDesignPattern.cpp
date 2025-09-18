// BRIDGE DESIGN PATTERN
// EXAMPLE --> CAR AND ENGINE

// #include "../../../builtin_files/bits-stdc++.h"
#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////////////////
// 					IENGINE (LOW LEVEL PART --> IMPLEMENTATION)
//////////////////////////////////////////////////////////////////

class IEngine {
public:
	virtual ~IEngine() {}
	virtual void start() = 0;
};

//////////////////////////////////////////////////////////////////
// 					CONCRETE ENGINES 
//////////////////////////////////////////////////////////////////

class ElectricEngine : public IEngine {
public:
	void start() override {
		cout<< "Electic engine powering up silently!" <<endl;
	}
};

class PetrolEngine : public IEngine {
public:
	void start() override {
		cout<< "Petrol engine starting with ignition!" <<endl;
	}
};

class DieselEngine : public IEngine {
public:
	void start() override {
		cout<< "Diesel engine roaring to life!" <<endl;
	}
};

//////////////////////////////////////////////////////////////////
// 					CAR (HIGH LEVEL PART --> ABSTRACTION)
//////////////////////////////////////////////////////////////////

class Car {
protected:
	shared_ptr<IEngine> engine;

public:
	Car(shared_ptr<IEngine>engine){
		this->engine = engine;
	}

	virtual void drive() = 0;
	virtual ~Car() {}
};

//////////////////////////////////////////////////////////////////
// 					CONCRETE CARS 
//////////////////////////////////////////////////////////////////

class SUV : public Car {
public:
	SUV(shared_ptr<IEngine>engine) : Car(engine) {}

	void drive() override {
		cout<< "Driving an SUV off-road" << endl;
		engine->start();
		cout<<endl;
	}
};

class Sedan : public Car {
public:
	Sedan(shared_ptr<IEngine>engine) : Car(engine) {}

	void drive() override {
		cout<< "Driving an Sedan on the highway" << endl;
		engine->start();
		cout<<endl;
	}
};


//////////////////////////////////////////////////////////////////
// 					MAIN / CLIENT
//////////////////////////////////////////////////////////////////

int main(){

	shared_ptr<IEngine> electricEng = make_shared<ElectricEngine>();
	shared_ptr<IEngine> petrolEng = make_shared<PetrolEngine>();
	shared_ptr<IEngine> dieselEng = make_shared<DieselEngine>();

	unique_ptr<Car> mySedan = make_unique<Sedan>(petrolEng);
	unique_ptr<Car> mySUV = make_unique<SUV>(electricEng);
	unique_ptr<Car> yourSUV = make_unique<SUV>(dieselEng);


	mySedan->drive();
	mySUV->drive();
	yourSUV->drive();

	return 0;
}
