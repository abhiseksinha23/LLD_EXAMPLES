// TEMPLATE METH0D PATTERN
// EXAMPLE --> MODEL TRAINING (FIXED EXECUTION --> TEMPLATE)

#include "../../../builtin_files/bits-stdc++.h"
using namespace std;

//////////////////////////////////////////////////////////////////
// 					METHODTRAINING 
//////////////////////////////////////////////////////////////////

class MethodTraining {
protected:
	virtual void load(const string& path) {
		cout<<"[COMMON] Loading the data from the file path : " << path << endl;
	}

	virtual void preProcess() {
		cout<<"[COMMON] Pre-Processing the data and cleaning the data !!!" << endl;
	}

	virtual void train()  = 0;
	virtual void evaluate() = 0;

	virtual void save() {
		cout<<"[COMMON] Saving the model into the files !!!" << endl;
	}
public:

	const void trainModel(const string& path){
		load(path);
		preProcess();
		train();
		evaluate();
		save();
	}
};

//////////////////////////////////////////////////////////////////
// 					NEURONETWORKMODEL 
//////////////////////////////////////////////////////////////////

class NeuroNetworkModel : public MethodTraining {
protected:
	void save() override {
		cout<<"[NEURONETWORK] Saving the model into the DB !!!" <<endl;
	}

	void train() override {
		cout<<"[NEURONETWORK] Training the data model !!!" <<endl;
	}

	void evaluate() override {
		cout<<"[NEURONETWORK] Evaluating the data model !!!" <<endl;	
	}
};


//////////////////////////////////////////////////////////////////
// 					DECISIONTREEMODEL 
//////////////////////////////////////////////////////////////////

class DecisionTreeModel : public MethodTraining {
protected:
	void train() override {
		cout<<"[DECISIONTREE] Training the data model !!!" <<endl;
	}

	void evaluate() override {
		cout<<"[DECISIONTREE] Evaluating the data model !!!" <<endl;	
	}
};


//////////////////////////////////////////////////////////////////
// 					CLIENT 
//////////////////////////////////////////////////////////////////

int main(){
	cout <<"====== Train the Neuro Network Model ============="<<endl;
	MethodTraining *neuronetwork = new NeuroNetworkModel();
	neuronetwork->trainModel("/data/file.txt");


	cout <<"====== Train the Decision Tree Model ============="<<endl;
	MethodTraining *decisionTree = new DecisionTreeModel();
	decisionTree->trainModel("/folder/content.csv");

	return 0;
}
