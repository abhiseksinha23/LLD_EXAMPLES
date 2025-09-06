// PROXY DESIGN PATTERN --> REMOTE PROXY
// EXAMPLE --> DATA SERVICE

#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////////////////
// 					IDATASERVICE 
//////////////////////////////////////////////////////////////////

class IDataService {
public:
	virtual void fetchData() = 0;
	virtual ~IDataService() {};
};

//////////////////////////////////////////////////////////////////
// 					DATASERVICE 
//////////////////////////////////////////////////////////////////

class DataService : public IDataService {
public:
    DataService(){
        cout<<"[DATASERVICE] OBJECT CREATED !!!"<<endl;
    }
	void fetchData() override {
		cout<<"[DATASERVICE] Fetching data over the internet from some other servers .... "<<endl;
		cout<<"[DATASERVICE] Data fetch completed !!!"<<endl;
	}
};

//////////////////////////////////////////////////////////////////
// 					DATAPROXY 
//////////////////////////////////////////////////////////////////

class DataProxy : public IDataService {
private:
	unique_ptr<DataService> dataService; 
public:
	DataProxy(){
		dataService = nullptr;
	}

	void fetchData() override {
		if(dataService == nullptr){
			dataService = make_unique<DataService>();
		}

		dataService->fetchData();
	}
};

//////////////////////////////////////////////////////////////////
// 					CLIENT 
//////////////////////////////////////////////////////////////////

int main() {
	shared_ptr<IDataService> dataService = make_shared<DataProxy>();
	dataService->fetchData();
	
	dataService->fetchData();


	return 0;
}

