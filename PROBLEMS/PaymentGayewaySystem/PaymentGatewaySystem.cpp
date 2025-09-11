// LLD PROBLEM -->
// PAYMENT GATEWAY SYSTEM

//////////////////////////////////////////////////////////////////
// 					REQUIREMENTS 
//////////////////////////////////////////////////////////////////
/*

FUNCTIONAL REQUIREMENTS

- SHOULD SUPPORT MULTIPLE PROVIDERS (PAYTM, RAZORPAY, ETC)
- WE CAN EASILY ADD NEW GATEWAYS IN FUTURE.
- THERE SHOULD BE A STANDARD PAYMENT FLOW, WITH REQUIRED VALIDATIONS
- HAVE ERROR HANDLING AND RETRIES MECHANISMS.
- THERE CAN BE DIFFERENT WAYS/STRATEGIES OF RETRY MECHANISM.
  --> LINEAR RETRY
  --> EXPONENTIAL BACK-OFF
 - TRY ADDING RECURRING PAYMENT FLOW/ SUBSCRIPTION


NON-FUNCTIONAL REQUIREMENTS

- SCALABLE
- PLUG N PLAY
- FOLLOW SOLID PRINCIPLES

*/
//////////////////////////////////////////////////////////////////

// #include "../../../builtin_files/bits-stdc++.h"
#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////////////////
// 					PAYMENTREQUEST
//////////////////////////////////////////////////////////////////

class PaymentRequest {
private:
	string sender, reciever, currency;
	double amount;
public:
	PaymentRequest(const string &sender, const string &reciever, const string &currency, double amount){
		this->sender = sender;
		this->reciever = reciever;
		this->currency = currency;
		this->amount = amount;
	}

	string getSender() {
		return sender;
	}

	string getReciever() {
		return reciever;
	}

	string getCurrency() {
		return currency;
	}

	double getAmount() {
		return  amount;
	}
};

//////////////////////////////////////////////////////////////////
// 					IBANKINGSYSTEM 
//////////////////////////////////////////////////////////////////

class IBankingSystem {
public:
	virtual bool processPayment(double amount) = 0;
	virtual ~IBankingSystem() {};
};


//////////////////////////////////////////////////////////////////
// 					PAYTMBAKINGSYSTEM
//////////////////////////////////////////////////////////////////

class PayTMBankingSystem : public IBankingSystem {
public:
	bool processPayment(double amount) override {
		cout<<"[PAYTM BANKING SYSTEM] Payment of amoount : "<< amount << " successful !!" << endl;
		int r = rand() % 100;
        return r < 20;
        //return false;
	}
};

//////////////////////////////////////////////////////////////////
// 					RAZORBANKINGSYSTEM 
//////////////////////////////////////////////////////////////////

class RazorPayBankingSystem : public IBankingSystem {
public:
	bool processPayment(double amount) override {
		cout<<"[RAZORPAY BANKING SYSTEM] Payment of amoount : "<< amount << " successful !!" << endl;
		int r = rand() % 100;
            return r < 90;
	}
};

//////////////////////////////////////////////////////////////////
// 					IPAYMENTGATEWAY 
//////////////////////////////////////////////////////////////////

class IPaymentGateway {
protected:
	shared_ptr<IBankingSystem> bankingSystem;
public:

	IPaymentGateway(){
		bankingSystem = nullptr;
	}

	virtual bool initiate(shared_ptr<PaymentRequest> &request) = 0;
	virtual bool validate(shared_ptr<PaymentRequest> &request) = 0;
	virtual bool confirm(shared_ptr<PaymentRequest> &request) = 0;

	virtual bool processPayment(shared_ptr<PaymentRequest> &request) {
		if(!validate(request)){
			cout<<"[PAYMENTGATEWAY] Validation failed for "<< request->getSender() <<".\n";
			return false;
		}

		if(!initiate(request)){
			cout<<"[PAYMENTGATEWAY] Initiation failed for "<< request->getSender() <<".\n";
			return false;
		}

		if(!confirm(request)){
			cout<<"[PAYMENTGATEWAY] Confirmation failed for "<< request->getSender() <<".\n";
			return false;
		}

		return true;
	}
};

//////////////////////////////////////////////////////////////////
// 					PAYTMPAYMENTGATEWAY 
//////////////////////////////////////////////////////////////////

class PayTMPaymentGateway : public IPaymentGateway {
public:
	PayTMPaymentGateway(){
		bankingSystem = make_shared<PayTMBankingSystem>();
	}


	bool validate(shared_ptr<PaymentRequest> &request) override {
		cout<<"[PAYTM PAYMENT GATEWAY] Initiating payment !!"<< request->getSender() << ".\n";

		if(request->getAmount() <=0 || request->getCurrency() != "INR") {
			return false;
		}
		return true;
	}

	bool initiate(shared_ptr<PaymentRequest> &request) override {
        cout << "[PAYTM PAYMENT GATEWAY] Initiating payment of " << request->getAmount() 
                  << " " << request->getCurrency() << " for " << request->getSender() << ".\n";

        return bankingSystem->processPayment(request->getAmount());
    }

    bool confirm(shared_ptr<PaymentRequest> &request) override {
        cout << "PAYTM PAYMENT GATEWAY] Confirming payment for " << request->getSender() << ".\n";

        // Confirmation always succeeds in this simulation
        return true;
    }
};

//////////////////////////////////////////////////////////////////
// 					RAZORPAYPAYMENTGATEWAY 
//////////////////////////////////////////////////////////////////

class RazorPayPaymentGateway : public IPaymentGateway {
public:
	RazorPayPaymentGateway(){
		bankingSystem = make_shared<RazorPayBankingSystem>();
	}

	bool validate(shared_ptr<PaymentRequest> &request) override {
		cout<<"[RAZORPAY PAYMENT GATEWAY] Initiating payment !!"<< request->getSender() << ".\n";

		if(request->getAmount() <=0) {
			return false;
		}
		return true;
	}

	bool initiate(shared_ptr<PaymentRequest> &request) override {
        cout << "[RAZORPAY PAYMENT GATEWAY] Initiating payment of " << request->getAmount() 
                  << " " << request->getCurrency() << " for " << request->getSender() << ".\n";

        return bankingSystem->processPayment(request->getAmount());
    }

    bool confirm(shared_ptr<PaymentRequest> &request) override {
        cout << "[RAZORPAY PAYMENT GATEWAY] Confirming payment for " << request->getSender() << ".\n";

        // Confirmation always succeeds in this simulation
        return true;
    }
};

//////////////////////////////////////////////////////////////////
// 					IRETRYSTRATEGY 
//////////////////////////////////////////////////////////////////

class IRetryStrategy {
public:
	virtual void retry() = 0;
};

//////////////////////////////////////////////////////////////////
// 					LINEARRETRYSTRATEGY 
//////////////////////////////////////////////////////////////////

class LinearRetryStrategy : public IRetryStrategy {
public:
	void retry() override {
		cout<<"Linear retry strategy being used." << endl;
		// add the logic
	}
};

//////////////////////////////////////////////////////////////////
// 					EXPONENTIALBACKOFFSTRATEGY
//////////////////////////////////////////////////////////////////

class ExponentialBackoffStrategy : public IRetryStrategy {
public:
	void retry() override {
		cout<<"Exponential Backoff retry strategy being used." << endl;
		// add the logic
	}
};

//////////////////////////////////////////////////////////////////
// 					PAYMENTGATEWAYPROXY
//////////////////////////////////////////////////////////////////

class PaymentGatewayProxy : public IPaymentGateway {
private:
	shared_ptr<IPaymentGateway> realGateway;
	shared_ptr<IRetryStrategy> retryStrategy;
	int retries;
public:
	PaymentGatewayProxy(shared_ptr<IPaymentGateway> &realGateway, shared_ptr<IRetryStrategy> &retryStrategy,  int retry){
		this->realGateway = realGateway;
		this->retryStrategy = retryStrategy;
		this->retries = retry;
	}

	 bool processPayment(shared_ptr<PaymentRequest> &request) override {
        bool result = false;
        for (int attempt = 0; attempt < retries; ++attempt) {
            if (attempt > 0) {
            	retryStrategy->retry();
                cout << "[Proxy] Retrying payment (attempt " << (attempt+1)
                          << ") for " << request->getSender() << ".\n";
            }
            result = realGateway->processPayment(request);
            if (result) break;
        }
        if (!result) {
            cout << "[Proxy] Payment failed after " << (retries)
                      << " attempts for " << request->getSender() << ".\n";
        }
        return result;
    }
    bool validate(shared_ptr<PaymentRequest> &request) override {
        return realGateway->validate(request);
    }
    bool initiate(shared_ptr<PaymentRequest> &request) override {
        return realGateway->initiate(request);
    }
    bool confirm(shared_ptr<PaymentRequest> &request) override {
        return realGateway->confirm(request);
    }	
};

//////////////////////////////////////////////////////////////////
// 					GATEWAYTYPE
//////////////////////////////////////////////////////////////////

enum class GatewayType {
	PAYTM,
	RAZORPAY
};

//////////////////////////////////////////////////////////////////
// 					PAYMENTGATEWAYFACTORY
//////////////////////////////////////////////////////////////////

class PaymentGatewayFactory {
private:
	static PaymentGatewayFactory instance;
	PaymentGatewayFactory(){}
public:
	static PaymentGatewayFactory& getInstance(){
		return instance;
	}

	shared_ptr<IPaymentGateway> getPaymentGateway(GatewayType type, shared_ptr<IRetryStrategy> retryStrategy) {
		if(type == GatewayType::PAYTM){
			
			shared_ptr<IPaymentGateway> paymentGateway = make_shared<PayTMPaymentGateway>();
			return make_shared<PaymentGatewayProxy>(paymentGateway, retryStrategy, 3);

		} else if (type == GatewayType::RAZORPAY){

			shared_ptr<IPaymentGateway> paymentGateway = make_shared<RazorPayPaymentGateway>();
			return make_shared<PaymentGatewayProxy>(paymentGateway, retryStrategy, 1);
			
		}else{
			cout<<"[PAYMENTGATEWAYFACTORY] Wrong type of Payment Gateway given." << endl;
			return nullptr;
		}
	}
};

PaymentGatewayFactory PaymentGatewayFactory::instance;


//////////////////////////////////////////////////////////////////
// 					PAYMENTSERVICE
//////////////////////////////////////////////////////////////////

class PaymentService {
private:
	static PaymentService instance;
	shared_ptr<IPaymentGateway> paymentGateway;
	PaymentService(){
		paymentGateway = nullptr;
	}

public:
	static PaymentService& getInstance(){
		return instance;
	}

	void setGateway(shared_ptr<IPaymentGateway> &paymentGateway){
		this->paymentGateway = paymentGateway;
	}

	bool processPayment(shared_ptr<PaymentRequest> &request){
		if (!paymentGateway) {
            cout << "[PaymentService] No payment gateway selected.\n";
            return false;
        }
		return paymentGateway->processPayment(request);
	}
};

PaymentService PaymentService::instance;


//////////////////////////////////////////////////////////////////
// 					PAYMENTCONTROLLER
//////////////////////////////////////////////////////////////////

class PaymentController {
private:
	static PaymentController instance;
	PaymentController(){}

public:
	static PaymentController& getInstance(){
		return instance;
	}

	bool processPayment( GatewayType type, shared_ptr<PaymentRequest> req, shared_ptr<IRetryStrategy> retryStrategy) {
		shared_ptr<IPaymentGateway> paymentGateway = PaymentGatewayFactory::getInstance().getPaymentGateway(type, retryStrategy);

		PaymentService::getInstance().setGateway(paymentGateway);

		return PaymentService::getInstance().processPayment(req);
	}
};

PaymentController PaymentController::instance;



//////////////////////////////////////////////////////////////////
// 					CLIENT
//////////////////////////////////////////////////////////////////

int main(){
	
	srand(static_cast<unsigned>(time(nullptr)));

   shared_ptr<PaymentRequest> req1 = make_shared<PaymentRequest>("Aditya", "Shubham", "INR", 1000.0);
   shared_ptr<IRetryStrategy> rs1 = make_shared<LinearRetryStrategy>();

    cout << "Processing via Paytm\n";
    cout << "------------------------------\n";
    bool res1 = PaymentController::getInstance().processPayment(GatewayType::PAYTM, req1, rs1);
    cout << "Result: " << (res1 ? "SUCCESS" : "FAIL") << "\n";
    cout << "------------------------------\n\n";

   shared_ptr<PaymentRequest> req2 = make_shared<PaymentRequest>("Shubham", "Aditya", "USD", 500.0);
   shared_ptr<IRetryStrategy> rs2 = make_shared<ExponentialBackoffStrategy>();
   
   
    cout << "Processing via Razorpay\n";
    cout << "------------------------------\n";
    bool res2 = PaymentController::getInstance().processPayment(GatewayType::RAZORPAY, req2, rs2);
    cout << "Result: " << (res2 ? "SUCCESS" : "FAIL") << "\n";
    cout << "------------------------------\n";

	return 0;
}
