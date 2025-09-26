// SIMPLE BUILDER DESIGN PATTERN USING SHARED POINTERS
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

class HttpRequestBuilder : public enable_shared_from_this<HttpRequestBuilder>{ // enable to share this from the class
private:
	shared_ptr<HttpRequest> req;

public:
	HttpRequestBuilder() : req(shared_ptr<HttpRequest>(new HttpRequest())){  // Assign it with a new shared_ptr as 
																			 // constructor is privste
	}

	shared_ptr<HttpRequestBuilder> withUrl(const string &url){
		req->url = url;
		return shared_from_this(); // returns this as a shared_ptr
	}

	shared_ptr<HttpRequestBuilder> withMethod(const string &method){
		req->method = method;
		return shared_from_this();
	}

	shared_ptr<HttpRequestBuilder> withHeader(const string &key, const string &value){
		req->headers[key] = value;
		return shared_from_this();
	}

	shared_ptr<HttpRequestBuilder> withQueryParams(const string &key, const string &value){
		req->queryParams[key] = value;
		return shared_from_this();
	}

	shared_ptr<HttpRequestBuilder> withBody(const string &body){
		req->body = body;
		return shared_from_this();
	}

	shared_ptr<HttpRequestBuilder> withTimeout(int timeout){
		req->timeout = timeout;
		return shared_from_this();
	}

	shared_ptr<HttpRequest> build(){
		// Validation logics can be added here
		if(req->url.empty()) {
			throw runtime_error("URL cannot be empty.");
		}
		return req;
	}

};


int main() {

	auto builder = make_shared<HttpRequestBuilder>(); // get the initialisation of req done first.

    shared_ptr<HttpRequest> req = builder
        ->withUrl("https://api.example.com")
        ->withMethod("POST")
        ->withHeader("Content-Type", "application/json")
        ->withHeader("Accept", "application/json")
        ->withQueryParams("key", "12345")
        ->withBody("{\"name\": \"Abhisek\"}")
        ->withTimeout(30)
        ->build();

	req->execute();

	return 0;
}
