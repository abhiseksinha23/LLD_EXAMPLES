// SIMPLE BUILDER DESIGN PATTERN
// EXAMPLE --> HTTP REQUEST

#include<bits/stdc++.h>
using namespace std;

class HttpRequest {
private:
	string url;                     // required
    string method;                  // required
    map<string, string> headers;
    map<string,string> queryParams;
    string body;
    int timeout;  

    HttpRequest() {} //private constructor

public:

	friend class HttpRequestBuilder;

	// Method to execute the HTTP request
    void execute() {
        cout << "Executing " << method << " request to " << url << endl;
        
        if (!queryParams.empty()) {
            cout << "Query Parameters:" << endl;
            for (const auto& param : queryParams) {
                cout << "  " << param.first << "=" << param.second << endl;
            }
        }

        cout << "Headers:" << endl;
        for (const auto& header : headers) {
            cout << "  " << header.first << ": " << header.second << endl;
        }
        
        if (!body.empty()) {
            cout << "Body: " << body << endl;
        }
        
        cout << "Timeout: " << timeout << " seconds" << endl;
        cout << "Request executed successfully!" << endl;
    }
};

class HttpRequestBuilder {
private:
	HttpRequest req;

public:

	HttpRequestBuilder& withUrl(const string &url){
		req.url = url;
		return *this;
	}

	HttpRequestBuilder& withMethod(const string &method){
		req.method = method;
		return *this;
	}

	HttpRequestBuilder& withHeader(const string &key, const string &value){
		req.headers[key] = value;
		return *this;
	}

	HttpRequestBuilder& withQueryParams(const string &key, const string &value){
		req.queryParams[key] = value;
		return *this;
	}

	HttpRequestBuilder& withBody(const string &body){
		req.body = body;
		return *this;
	}

	HttpRequestBuilder& withTimeout(int timeout){
		req.timeout = timeout;
		return *this;
	}

	HttpRequest build(){
		// Validation logics can be added here
		if(req.url.empty()) {
			throw runtime_error("URL cannot be empty.");
		}
		return req;
	}
};

int main() {

	HttpRequest req = HttpRequestBuilder()
		.withUrl("https://api.example.com")
		.withMethod("POST")
		.withHeader("Content-Type", "application/json")
		.withHeader("Accept", "application/json")
		.withQueryParams("key", "12345")
		.withBody("{\"name\": \"Abhisek\"}")
		.withTimeout(30)
		.build();

	req.execute();

	return 0;
}