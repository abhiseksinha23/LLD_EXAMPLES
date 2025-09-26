// STEP BUILDER DESIGN PATTERN
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

	HttpRequest() {}
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

// FORWARD DECLARATION
class MethodStep;
class HeaderStep;
class OptionalStep;


class UrlStep {
public:
	virtual MethodStep& withUrl(const string &url) = 0;
};

class MethodStep {
public:
	virtual HeaderStep& withMethod(const string &method) = 0;
};

class HeaderStep {
public:
	virtual OptionalStep& withHeader(string key, string value) = 0;
};

class OptionalStep {
public:
	virtual OptionalStep& withBody(const string &body) = 0;
	virtual OptionalStep& withTimeout(int timeout) = 0;
	virtual OptionalStep& withQueryParams(string key, string value) = 0;
	virtual HttpRequest build() = 0;
};

class HttpRequestBuilder : 
	public UrlStep, 
	public MethodStep, 
	public HeaderStep, 
	public OptionalStep {
private:
	HttpRequest req;

public:

	static UrlStep& getBuilder() {
		return *(new HttpRequestBuilder());
	}

	MethodStep& withUrl(const string &url) override {
		req.url = url;
		return *this;
	}

	HeaderStep& withMethod(const string &method) override {
		req.method = method;
		return *this;
	}

	OptionalStep& withHeader(string key, string value) override {
		req.headers[key] = value;
		return *this;
	}

	OptionalStep& withBody(const string &body) override {
		req.body = body;
		return *this;
	}

	OptionalStep& withTimeout(int timeout) override {
		req.timeout = timeout;
		return *this;
	}

	OptionalStep& withQueryParams(string key, string value) override {
		req.headers[key] = value;
		return *this;
	}

	HttpRequest build() {
		return req;
	}	
};

int main() {
	HttpRequest stepRequest = HttpRequestBuilder::getBuilder()
		.withUrl("https://api.example.com")
		.withMethod("POST")
		.withHeader("Content-Type", "application/json")
		.withQueryParams("key", "12345")
		.withBody("{\"name\": \"Abhisek\"}")
		.withTimeout(30)
		.build();

	stepRequest.execute();

	return 0;
}
