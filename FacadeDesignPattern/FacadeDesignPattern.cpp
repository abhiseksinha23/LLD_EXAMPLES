// FACADE DESING PATTERN EXAMPLE --> COMPUTER POWER ON

/*
Client interacts with ComputerFacade class and rest 
all is handled by the ComputerFacade class itself.
*/

//#include "../../../builtin_files/bits-stdc++.h"
#include<bits/stdc++.h>
using namespace std;

////////////////////////////////////////////////////
//   				SUBSYSTEM
////////////////////////////////////////////////////

class CPU {
public:
	void initialise(){
		cout<<"CPU initialised" << endl;
	}
};

class Memory {
public:
	void selfTest(){
		cout<<"Memory self Test completed, result --> PASSED." << endl;
	}
};

class OperatingSystem {
public:
	void load(){
		cout<<"Operating System loaded" << endl;
	}
};

class HardDrive {
public:
	void spinUp(){
		cout<<"HardDrive started spinning...." << endl;
	}
};

class CoolingSystem {
public:
	void startFan(){
		cout<<"Fan started and running...." << endl;
	}
};

class PowerSupply {
public:
	void providePower(){
		cout<<"Power supply provided...." << endl;
	}
};


class BIOS {
private:
	CPU *cpu;
	Memory *memory;
public:
	void boot(CPU *cpu, Memory *memory){
		this->cpu = cpu;
		this->memory = memory;

		cout<<"booting up the system...." << endl;
		cout<< endl;
		this->cpu->initialise();
		cout<< endl;
		this->memory->selfTest();

		cout<< endl;
		cout<<"System booted !!" << endl;
	}
};


////////////////////////////////////////////////////
//   				COMPUTER FACADE
////////////////////////////////////////////////////

class ComputerFacade {
private:
    CPU *cpu;
    Memory *memory;
    OperatingSystem *os;
    HardDrive *hd;
    CoolingSystem *cs;
    PowerSupply *ps;
    BIOS *bios;

public:
	ComputerFacade(){
        cpu = new CPU();
        memory = new Memory();
        os = new OperatingSystem();
        hd = new HardDrive();
        cs = new CoolingSystem();
        ps = new PowerSupply();
        bios = new BIOS();	
	}

	void startComputer() {
		cout<<"Starting the computer...." << endl;
		cout<< endl;
        ps->providePower();
        cout<< endl;
        cs->startFan();
        cout<< endl;
        hd->spinUp();
        cout<< endl;
        bios->boot(cpu, memory);
        cout<< endl;
        os->load();
        cout<< endl;

        cout<<"Computer started successfully !!" << endl;
	}
};


////////////////////////////////////////////////////
//   				CLIENT/ MAIN
////////////////////////////////////////////////////

int main(){
	// ComputerFacade *computer = new ComputerFacade(); 
	// computer->startComputer();

	ComputerFacade computer; 
	computer.startComputer();

	return 0;
}
