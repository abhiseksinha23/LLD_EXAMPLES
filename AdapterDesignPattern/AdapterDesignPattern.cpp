// ADAPTOR DESING PATTERN EXAMPLE --> 3RD PARTY INTEGRATION

/*
Client needs JSON data from a string
but using a 3rd party it able to get the data in XML format from that string 
and then using the adapter it converts the XML to JSON
*/

//#include "../../../builtin_files/bits-stdc++.h"
#include<bits/stdc++.h>
using namespace std;

////////////////// IREPORT /////////////////////

class IReport {
public:
	virtual string getJsonData(const string &data) = 0;
	virtual ~IReport() {}
};

////////////////// XMLDATAPROVIDER /////////////////////

class XMLDataProvider {
public:
	// takes string as ALice:42
	string getXMLData(const string &data) {
		size_t sep = data.find(':');
		string name = data.substr(0, sep);
		string id = data.substr(sep + 1);

		return "<user>"
				"<name>" + name + "</name>"
				"<id>" + id + "</id>"
				"</user>";
	}
};


////////////////// XMLDATAPROVIDERADAPTER /////////////////////

class XMLDataProviderAdapter : public IReport {
private:
	XMLDataProvider *provider;

public:
	XMLDataProviderAdapter(XMLDataProvider *provider){
		this->provider = provider;
	}

	string getJsonData(const string &data) override {
		string xml = provider->getXMLData(data);

		size_t startName = xml.find("<name>") + 6;
		size_t endName = xml.find("</name>");
		string name = xml.substr(startName, endName - startName);

		size_t startID = xml.find("<id>") + 4;
		size_t endID = xml.find("</id>");
		string id = xml.substr(startID, endID - startID);


		return "{\"name\": \"" + name + "\", \"id\":" + id + "}";
	}
};

///////////////// CLIENT /////////////////////

class Client {
private:
	IReport *report;
public:
	Client(IReport *report){
		this->report = report;
	}

	void getJsonData(const string &data){
		cout<<"Processed JSON : "
		<< report->getJsonData(data)
		<<endl;
	}

	void setReport(IReport *report){
		this->report = report;
	}

	IReport* getReport(){
		return report;
	}
};


///////////////// MAIN /////////////////////

int main() {
	XMLDataProvider *XMLProvider = new XMLDataProvider();

	IReport *adapter = new XMLDataProviderAdapter(XMLProvider);
	// unique_ptr<IReport> adap = make_unique<XMLDataProviderAdapter>(XMLProvider);

	Client *client = new Client(adapter);

	string raw_data = "ALICE:23";
	client->getJsonData(raw_data);


	//////////////// XMLDATAPROVIDER'S VALUE ///////////////////////
	cout<< XMLProvider->getXMLData(raw_data) << endl;

	delete client;
	delete adapter;
	delete XMLProvider;

	return 0;
}
