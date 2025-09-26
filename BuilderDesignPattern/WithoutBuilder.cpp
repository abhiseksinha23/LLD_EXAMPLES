// BUILDER DESIGN PATTERN
// HOW THE CODE LOOKS WE DON'T USE THE BUILDER PATTERN

// BAD CODE EXAMPLE
//////////////////////////////////////////////////////////////////

#include<bits/stdc++.h>
using namespace std;

class HttpRequest {
private:
    string url;                     // required
    string method;                  // required
    map<string, string> headers;
    map<string,string> queryParams;
    string body;
    int timeout;                    // required
    
public:
    // Constructor with only required parameter (1-arg)
    HttpRequest(const string& url) {
        this->url = url;
        this->method = "GET";       // Default method
        this->timeout = 30;         // Default timeout
    }
    
    // 2 - args
    HttpRequest(const string& url, string method) {
        this->url = url;
        this->method = method;
        this->timeout = 30;
    }
    
    // 3 - args
    HttpRequest(const string& url, string method, int timeout) {
        this->url = url;
        this->method = method;
        this->timeout = timeout;
    }
    
    // 4 - args
    HttpRequest(const string& url, string method, int timeout, map<string, string> headers) {
        this->url = url;
        this->method = method;
        this->timeout = timeout;
        this->headers = headers;
    }

    // 5 - args
    HttpRequest(const string& url, string method, int timeout, 
        map<string, string> headers, map<string,string> queryParams) {
        this->url = url;
        this->method = method;
        this->timeout = timeout;
        this->headers = headers;
        this->queryParams = queryParams;
    }

    // 6 - args
    HttpRequest(const string& url, string method, int timeout, 
        map<string, string> headers, map<string,string> queryParams, string body) {
        this->url = url;
        this->method = method;
        this->timeout = timeout;
        this->headers = headers;
        this->queryParams = queryParams;
        this->body = body;
    }
    
    // Setters (leads to mutable object)
    void setUrl(const string& url) {
        this->url = url;
    }
    
    void setMethod(string method) {
        this->method = method;
    }
    
    void addHeader(const string& key, const string& value) {
        headers[key] = value;
    }

    void addQueryParam(const string& key, const string& value) {
        queryParams[key] = value;
    }
    
    void setBody(const string& body) {
        this->body = body;
    }
    
    void setTimeout(int timeout) {
        this->timeout = timeout;
    }
    
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

int main() {
    // Using constructors (telescoping constructor problem)
    unique_ptr<HttpRequest> request1 = make_unique<HttpRequest>("https://api.example.com");
    unique_ptr<HttpRequest> request2 = make_unique<HttpRequest>("https://api.example.com", "POST");
    unique_ptr<HttpRequest> request3 = make_unique<HttpRequest>("https://api.example.com", "PUT", 60);
    
    // Using setters (mutable object problem)
    unique_ptr<HttpRequest> request4 = make_unique<HttpRequest>("https://api.example.com");
    request4->setMethod("POST");
    request4->addHeader("Content-Type", "application/json");
    request4->addQueryParam("key", "12345");

    //request4->execute();

    request4->setBody("{\"name\": \"Abhisek\"}");
    request4->setTimeout(60);
    
    // The problem: what if we forgot to set an important field?
    request4->execute();
}
