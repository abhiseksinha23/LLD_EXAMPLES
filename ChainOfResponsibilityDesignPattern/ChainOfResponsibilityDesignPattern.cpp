// CHAIN OF RESPONSIBILTY DESIGN PATTERN
// EXAMPLE --> ATM DISPENSER

// #include "../../../builtin_files/bits-stdc++.h"
#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////////////////
// 					IHANDLER 
//////////////////////////////////////////////////////////////////

class IHandler {
protected:
	shared_ptr<IHandler> nextHandler;

public:
	IHandler(){
		nextHandler = nullptr;
	}

	virtual void dispense(int amount) = 0;

	virtual void setNextHandler(shared_ptr<IHandler> &handler){
		this->nextHandler = handler;
	}
};


//////////////////////////////////////////////////////////////////
// 					THOUSANDHANDLER 
//////////////////////////////////////////////////////////////////

class ThousandHandler : public IHandler{
private:
	int totalNotes;
public:
	ThousandHandler(int notes){
		this->totalNotes = notes;
	}

	void increaseNotes(int notes){
		this->totalNotes += notes;
	}

	void dispense(int amount) override {
		int notesNeeded = amount/ 1000;

		if(notesNeeded > totalNotes){
			notesNeeded = totalNotes;
			totalNotes = 0;
		} else {
			totalNotes -= notesNeeded;
		}

		if(notesNeeded > 0) {
			cout<<"Dispensing " << notesNeeded <<" x 1000 note(s)." <<endl;
		}


		int remainingAmount = amount - (notesNeeded * 1000);
		if(remainingAmount > 0) {
			if(nextHandler != nullptr){
				nextHandler->dispense(remainingAmount);
			} else {
				cout<<"Remaining Amount "<< remainingAmount <<" cannont be fulfilled." <<endl; 
			}
		}
	}
};


//////////////////////////////////////////////////////////////////
// 					FIVEHUNDREDDISPENSER 
//////////////////////////////////////////////////////////////////

class FiveHundredHandler : public IHandler{
private:
    int totalNotes;
public:
    FiveHundredHandler(int notes){
        this->totalNotes = notes;
    }

    void increaseNotes(int notes){
        this->totalNotes += notes;
    }

    void dispense(int amount) override {
        int notesNeeded = amount / 500;

        if(notesNeeded > totalNotes){
            notesNeeded = totalNotes;
            totalNotes = 0;
        } else {
            totalNotes -= notesNeeded;
        }

        if(notesNeeded > 0) {
            cout<<"Dispensing " << notesNeeded <<" x 500 note(s)." <<endl;
        }

        int remainingAmount = amount - (notesNeeded * 500);
        if(remainingAmount > 0) {
            if(nextHandler != nullptr){
                nextHandler->dispense(remainingAmount);
            } else {
                cout<<"Remaining Amount "<< remainingAmount <<" cannont be fulfilled." <<endl; 
            }
        }
    }
};

//////////////////////////////////////////////////////////////////
// 					TWOHUNDREDDISPENSER 
//////////////////////////////////////////////////////////////////

class TwoHundredHandler : public IHandler{
private:
    int totalNotes;
public:
    TwoHundredHandler(int notes){
        this->totalNotes = notes;
    }

    void increaseNotes(int notes){
        this->totalNotes += notes;
    }

    void dispense(int amount) override {
        int notesNeeded = amount / 200;

        if(notesNeeded > totalNotes){
            notesNeeded = totalNotes;
            totalNotes = 0;
        } else {
            totalNotes -= notesNeeded;
        }

        if(notesNeeded > 0) {
            cout<<"Dispensing " << notesNeeded <<" x 200 note(s)." <<endl;
        }

        int remainingAmount = amount - (notesNeeded * 200);
        if(remainingAmount > 0) {
            if(nextHandler != nullptr){
                nextHandler->dispense(remainingAmount);
            } else {
                cout<<"Remaining Amount "<< remainingAmount <<" cannont be fulfilled." <<endl; 
            }
        }
    }
};

//////////////////////////////////////////////////////////////////
// 					HUNDREDDISPENSER 
//////////////////////////////////////////////////////////////////

class HundredHandler : public IHandler{
private:
    int totalNotes;
public:
    HundredHandler(int notes){
        this->totalNotes = notes;
    }
    void increaseNotes(int notes){
        this->totalNotes += notes;
    }
    void dispense(int amount) override {
        int notesNeeded = amount / 100;

        if(notesNeeded > totalNotes){
            notesNeeded = totalNotes;
            totalNotes = 0;
        } else {
            totalNotes -= notesNeeded;
        }

        if(notesNeeded > 0) {
            cout<<"Dispensing " << notesNeeded <<" x 100 note(s)." <<endl;
        }

        int remainingAmount = amount - (notesNeeded * 100);
        if(remainingAmount > 0) {
            if(nextHandler != nullptr){
                nextHandler->dispense(remainingAmount);
            } else {
                cout<<"Remaining Amount "<< remainingAmount <<" cannont be fulfilled." <<endl; 
            }
        }
    }
};

//////////////////////////////////////////////////////////////////
// 					CLIENT 
//////////////////////////////////////////////////////////////////

int main(){
	unique_ptr<IHandler> thousandHanlder = make_unique<ThousandHandler>(3);
	shared_ptr<IHandler> fiveHundredHandler = make_shared<FiveHundredHandler>(5);
	shared_ptr<IHandler> twoHundredHandler = make_shared<TwoHundredHandler>(10);
	shared_ptr<IHandler> hundredHandler = make_shared<HundredHandler>(20);


	thousandHanlder->setNextHandler(fiveHundredHandler);
	fiveHundredHandler->setNextHandler(twoHundredHandler);
	twoHundredHandler->setNextHandler(hundredHandler);


	int amountToWithdraw = 4300;

	cout<<"\nDispatching amount Rs." << amountToWithdraw << endl;
	thousandHanlder->dispense(amountToWithdraw);

	return 0;
}
