// STATE DESIGN PATTERN
// EXAMPLE --> VENDING MACHINE

#include<bits/stdc++.h>
using namespace std;


// Forward declartions

class VendingMachine;
class NoCoinState;
class HasCoinState;
class DispenseState;
class SoldOutState;


class VendingState {
public:
	virtual shared_ptr<VendingState> insertCoin(VendingMachine* machine, int coins) = 0;
	virtual shared_ptr<VendingState> selectItem(VendingMachine* machine) = 0;
	virtual shared_ptr<VendingState> dispense(VendingMachine* machine) = 0;
	virtual shared_ptr<VendingState> returnCoin(VendingMachine* machine) = 0;
	virtual shared_ptr<VendingState> refill(VendingMachine* machine, int quantity) = 0;
	virtual string getStateName() = 0;
};

class VendingMachine {
private:
	shared_ptr<VendingState> currentState;
	int itemCount;
	int itemPrice;
	int insertedCoins;

	shared_ptr<VendingState> noCoinState;
	shared_ptr<VendingState> hasCoinState;
	shared_ptr<VendingState> dispenseState;
	shared_ptr<VendingState> soldOutState;

public:

	VendingMachine(int itemCount, int itemPrice);

	void insertCoin(int coin);
	void selectItem();
	void dispense();
	void returnCoin();
	void refill(int quantity);

	void printStatus();

	shared_ptr<VendingState> getNoCoinState() {
		return noCoinState;
	}

	shared_ptr<VendingState> getHasCoinState() {
		return hasCoinState;
	}

	shared_ptr<VendingState> getDispenseState() {
		return dispenseState;
	}

	shared_ptr<VendingState> getSoldOutstate() {
		return soldOutState;
	}

	int getItemCount() { 
        return itemCount; 
    }
    void decrementItemCount() { 
        itemCount--; 
    }
    void incrementItemCount(int count = 1) {
        itemCount += count;
    }
    int getInsertedCoin() { 
        return insertedCoins;
    }
    void setInsertedCoin(int coin) { 
        insertedCoins = coin;
    }
    void addCoin(int coin) { 
        insertedCoins += coin;
    }
    int getPrice() {
        return this->itemPrice;
    }
    void setPrice(int itemPrice) {
        this->itemPrice = itemPrice;
    }
};



class NoCoinState : public VendingState {
public:
	shared_ptr<VendingState> insertCoin(VendingMachine* machine, int coin) override {
		machine->setInsertedCoin(coin);
		cout << "Coin inserted. Current balance: Rs " << coin <<endl;
		return machine->getHasCoinState();
	}

	shared_ptr<VendingState> selectItem(VendingMachine* machine) override {
		cout << "Please insert coin first!" <<endl;
		return machine->getNoCoinState();
	}

	shared_ptr<VendingState> dispense(VendingMachine* machine) override {
		cout << "Please insert coin and select item first!" <<endl;
		return machine->getNoCoinState();	
	}
	
	shared_ptr<VendingState> returnCoin(VendingMachine* machine) override {
		cout << "No coin to return!" <<endl;
		return machine->getNoCoinState();
	}

	shared_ptr<VendingState> refill(VendingMachine* machine, int quantity) override {
		cout << "Items refilling" <<endl;
		machine->incrementItemCount(quantity);
		return machine->getNoCoinState();
	}

	string getStateName() {
		return "NO_COIN";
	}
};


class HasCoinState : public VendingState {
public:
	shared_ptr<VendingState> insertCoin(VendingMachine* machine, int coin) override {
		machine->addCoin(coin);
		cout << "Additional coin inserted. Current balance: Rs " << machine->getInsertedCoin() <<endl;
		return machine->getHasCoinState();
	}

	shared_ptr<VendingState> selectItem(VendingMachine* machine) override {

		if(machine->getInsertedCoin() >= machine->getPrice()) {
			cout<<"Item selected, Dispensing ..." <<endl;

			int change = machine->getInsertedCoin() - machine->getPrice();
			
			if(change > 0) {
				cout << "Change returned: Rs " << change <<endl;
			}

			machine->setInsertedCoin(0);
			return machine->getDispenseState();
		}

		int needed = machine->getPrice() - machine->getInsertedCoin();
        cout << "Insufficient funds. Need Rs " << needed << " more." <<endl;
        return machine->getHasCoinState();		
		
	}

	shared_ptr<VendingState> dispense(VendingMachine* machine) override {
		cout << "Please select an item first!" <<endl;
		return machine->getHasCoinState();	
	}
	
	shared_ptr<VendingState> returnCoin(VendingMachine* machine) override {
		cout << "Coin returned: Rs " << machine->getInsertedCoin() <<endl;
        machine->setInsertedCoin(0);
		return machine->getNoCoinState();
	}

	shared_ptr<VendingState> refill(VendingMachine* machine, int quantity) override {
		cout << "Can't refill in this state" <<endl;
		return machine->getHasCoinState();
	}

	string getStateName() {
		return "HAS_COIN";
	}
};


class DispenseState : public VendingState {
public:
	shared_ptr<VendingState> insertCoin(VendingMachine* machine, int coin) override {
		cout << "Please wait, already dispensing item. Coin returned: Rs " << coin <<endl;
		return machine->getDispenseState();
	}

	shared_ptr<VendingState> selectItem(VendingMachine* machine) override {
		cout << "Already dispensing item. Please wait." <<endl;
		return machine->getDispenseState();
	}

	shared_ptr<VendingState> dispense(VendingMachine* machine) override {
		cout << "Item dispensed!" <<endl;
		machine->decrementItemCount();

		if(machine->getItemCount() == 0) {
			return machine->getSoldOutstate();	
		}

		return machine->getNoCoinState();
	}
	
	shared_ptr<VendingState> returnCoin(VendingMachine* machine) override {
		cout << "Cannot return the coin while dispensing item!" <<endl;
		return machine->getDispenseState();
	}

	shared_ptr<VendingState> refill(VendingMachine* machine, int quantity) override {
		cout << "Can't refill in this state" <<endl;
		return machine->getDispenseState();
	}

	string getStateName() {
		return "DISPENSING";
	}
};


class SoldOutState : public VendingState {
public:
	shared_ptr<VendingState> insertCoin(VendingMachine* machine, int coin) override {
		cout << "Machine is sold out. Coin returned: Rs " << coin <<endl;
		return machine->getSoldOutstate();
	}

	shared_ptr<VendingState> selectItem(VendingMachine* machine) override {
		cout << "Machine is sold out!" <<endl;
		return machine->getSoldOutstate();
	}

	shared_ptr<VendingState> dispense(VendingMachine* machine) override {
		cout << "Machine is sold out!" <<endl;
		return machine->getSoldOutstate();
	}
	
	shared_ptr<VendingState> returnCoin(VendingMachine* machine) override {
		cout << "Machine is sold out, no coin inserted." <<endl;
		return machine->getSoldOutstate();
	}

	shared_ptr<VendingState> refill(VendingMachine* machine, int quantity) override {
		cout << "Items refilling..." <<endl;
		machine->incrementItemCount(quantity);
		return machine->getNoCoinState();
	}

	string getStateName() {
		return "SOLD_OUT";
	}
};





// Vending Machine implementation
VendingMachine::VendingMachine(int itemCount, int itemPrice) {
	this->itemCount = itemCount;
	this->itemPrice = itemPrice;
	insertedCoins = 0;

	noCoinState = make_shared<NoCoinState>();
	hasCoinState = make_shared<HasCoinState>();
	dispenseState = make_shared<DispenseState>();
	soldOutState = make_shared<SoldOutState>();
	
	currentState = noCoinState;
}


void VendingMachine::insertCoin(int coins) {
	currentState = currentState->insertCoin(this, coins);
}

void VendingMachine::selectItem() {
	currentState = currentState->selectItem(this);
}

void VendingMachine::dispense() {
	currentState = currentState->dispense(this);
}

void VendingMachine::returnCoin() {
	currentState = currentState->returnCoin(this);
}

void VendingMachine::refill(int quantity) {
	currentState = currentState->refill(this, quantity);
}

void VendingMachine::printStatus() {
	cout << "\n--- Vending Machine Status ---" << endl;
    cout << "Items remaining: " << itemCount << endl;
    cout << "Inserted coin: Rs " << insertedCoins << endl;
    cout << "Current state: " << currentState->getStateName()<< endl << endl;
}


int main() {
    cout << "=== Water Bottle VENDING MACHINE ===" <<endl;
    
    int itemCount = 2;
    int itemPrice = 20;

    VendingMachine machine(itemCount, itemPrice);
    machine.printStatus();
    
    // Test scenarios - each operation potentially changes state
    cout << "1. Trying to select item without coin:" <<endl;
    machine.selectItem();  // Should ask for coin, no state change
    machine.printStatus();
    
    cout << "2. Inserting coin:" <<endl;
    machine.insertCoin(10);  // State changes to HAS_COIN
    machine.printStatus();
    
    cout << "3. Selecting item with insufficient funds:" <<endl;
    machine.selectItem();  // Insufficient funds, stays in HAS_COIN
    machine.printStatus();
    
    cout << "4. Adding more coins:" <<endl;
    machine.insertCoin(10);  // Add more money, stays in HAS_COIN
    machine.printStatus();
    
    cout << "5. Selecting item Now" <<endl;
    machine.selectItem();  // State changes to SOLD
    machine.printStatus();
    
    cout << "6. Dispensing item:" <<endl;
    machine.dispense(); // State changes to NO_COIN (items remaining)
    machine.printStatus();
    
    cout << "7. Buying last item:" <<endl;
    machine.insertCoin(20);  // State changes to HAS_COIN
    machine.selectItem();  // State changes to SOLD
    machine.dispense(); // State changes to SOLD_OUT (no items left)
    machine.printStatus();
    
    cout << "8. Trying to use sold out machine:" <<endl;
    machine.insertCoin(5);  // Coin returned, stays in SOLD_OUT

    cout<<endl;
    cout << "9. Trying to use sold out machine:" <<endl;
    machine.refill(2);
    machine.printStatus(); // State changes NO_COIN
    
	return 0;
}
