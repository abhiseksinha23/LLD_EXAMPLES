/*
Q> Design an Online Food Ordering application.


Functional Requirements-
1. User can search the restaurants via location.
2. User can add item to the cart.
3. User can place an order by making the payment.
4. User should get the notification.


Non-Functional Requirements-
1. Code reusable


Class/Object -
1. Restaurants
2. User
3. Cart
4. Order
5. Payment
6. Notification

*/

#include <bits/stdc++.h>
//#include "../../../builtin_files/bits-stdc++.h"
using namespace std;

///////////////////////////// MENUITEM ////////////////////////////
class MenuItem {
private:
	int id;
	string name;
	double price;
	static int nextId;

public:
	MenuItem(string name, double price){
		this->name = name;
		this->price = price;
		this->id = ++nextId;
	}

	string getName(){
		return name;
	}

	double getPrice(){
		return price;
	}

	void setName(string name){
		this->name = name;
	}

	void setPrice(double price){
		this->price = price;
	}
};

int MenuItem::nextId = 0;

///////////////////////////// RESTAURANT ////////////////////////////
class Restaurant {
private:
	int id;
	static int nextId;
	string name;
	string address;
	vector<MenuItem*> menu;

public:
	Restaurant(string name, string address){
		this->id = ++nextId;
		this->name = name;
		this->address = address;
	}

	void addItem(MenuItem *item){
		menu.push_back(item);
	}

	string getName(){
		return name;
	}

	string getAddress() {
		return address;
	}

	vector<MenuItem*> getMenu() {
		return menu;
	}
};

int Restaurant::nextId = 0;


/////////////////// RESTAURANT MANAGER ////////////////////////////
/////////////////////// SINGLETON CLASS ///////////////////////////
class RestaurantManager {
private:
	static RestaurantManager *instance;
	static mutex mtx;
	vector<Restaurant*> restaurants;

	RestaurantManager(){
		cout<<"RestaurantManager instance created" << endl;
	}

public:
	static RestaurantManager* getInstance(){
		if(instance == nullptr){
			lock_guard<mutex> lock(mtx);
			if(instance == nullptr)
				instance = new RestaurantManager();
		}
		return instance;
	}

	void addRestaurants(Restaurant * res){
		restaurants.push_back(res);
	}

	vector<Restaurant*> getRestaurants(string &loc){
		vector<Restaurant*> result;
		for(auto res : restaurants){
			if(res->getAddress() == loc){
				result.push_back(res);
			}
		}
		return result;
	}
};

RestaurantManager* RestaurantManager::instance = nullptr;
mutex RestaurantManager::mtx;

////////////////////// CART ////////////////////////////////
class Cart {
private:
	Restaurant* rs;
	vector<MenuItem*>items;
	double totalPrice;

public:
	Cart(){
		totalPrice = 0;
		items.resize(0);
	}

	void clearCart(){
		items.resize(0);
		totalPrice = 0;
	}

	void addRestaurant(Restaurant *rs){
		if(this->rs == rs) return;
		this->rs = rs;
		clearCart();
	}

	void addItem(MenuItem *item){
		items.push_back(item);
		totalPrice += item->getPrice();
	}

	vector<MenuItem*> getItems() {
		return items;
	}

	Restaurant* getRestaurant() {
		return rs;
	}
};

////////////////////// NOTIFICATION SYSTEM ///////////////////////
//// INTERFACE ////
class NotificationService {
public:
	virtual void notify() = 0;
};

class SMS : public NotificationService {
public:
	void notify() override {
		cout<<"SMS SENT TO USER"<<endl;
	}
};

class Email : public NotificationService {
public:
	void notify() override {
		cout<<"Emai; SENT TO USER"<<endl;
	}
};

////////////////////// PAYMENT STRATEGY ///////////////////////
//// INTERFACE ////
class PaymentStrategy {
public:
	virtual bool pay() = 0;
};

class UPI : public PaymentStrategy {
public:
	bool pay() override {
		cout<<"PAYMENT MADE VIA UPI -- SUCCESS "<<endl;
		return true;
	}
};

class CC : public PaymentStrategy {
public:
	bool pay() override {
		cout<<"PAYMENT MADE VIA CC -- SUCCESS "<<endl;
		return true;
	}
};

class NetBanking : public PaymentStrategy {
public:
	bool pay() override {
		cout<<"PAYMENT MADE VIA NetBanking -- SUCCESS "<<endl;
		return true;
	}
};


////////////////////// USER ////////////////////////////////
class User {
private:
	int id;
	static int nextId;
	string name;
	string address;
	Cart *cs;

public:

	User(string name, string address){
		this->name = name;
		this->address = address;
		cs = new Cart();
	}

	void addToCart(Restaurant *rs, MenuItem *item){
		cs->addRestaurant(rs);
		cs->addItem(item);
	}
	string getName() {
		return name;
	}
	string getAddress() {
		return address;
	}
	void setName(string name) {
		this->name = name;
	}
	void setAddress(string address) {
		this->address = address;
	}
	Cart* getCart() {
		return cs;
	}
	int getId() {
		return id;
	}

};

int User::nextId = 0;

////////////////////// ORDER ////////////////////////////////
/// INTERFACE///
class Order {
public:
	int id;
	static int nextId;
	User *user;
	Restaurant *rs;
	vector<MenuItem*>items;
	double totalPrice;

	NotificationService *ns;
	PaymentStrategy *ps;
	string type;

	virtual void createOrder(User *user, Restaurant *rs, 
							   vector<MenuItem*>items, double totalPrice,
							   NotificationService *ns, PaymentStrategy *ps) = 0;

	virtual void displayOrder() = 0;

};

int Order::nextId = 0;

class DeliveryOrder : public Order {
private:
	string address;

public:
	void setAddress(string address){
		this->address = address;
	}

	void createOrder(User *user, Restaurant *rs, 
							   vector<MenuItem*>items, double totalPrice,
							   NotificationService *ns, PaymentStrategy *ps) override {
		this->id = ++nextId;
		this->user = user;
		this->rs = rs;
		this->items = items;
		this->totalPrice = totalPrice;
		this->ns = ns;
		this->ps = ps;
		this->type = "Delivery";
	}

	void displayOrder() override {
		cout<<"----------- ORDER'S DETAILS ----------------"<<endl;

		cout<<" Order's ID : "<< id << endl;
		cout<<" User's Name : "<< user->getName() << endl;
		cout<<" User's Address : "<< address << endl;
		cout<<" Restaurant's Name : "<< rs->getName() << endl;
		cout<<" Restaurant's Address : "<< rs->getAddress() << endl;
		cout<<" Items Ordered : "<< endl;
		for(auto item : items){
			cout<<" - " << item->getName() << " : $" << item->getPrice() << endl;
		}
		cout<<" Total Price : $" << totalPrice << endl;
	}
};

class PickupOrder : public Order {
private:
	string rsAddress;

public:
	
	void setRsAddress(string address){
		this->rsAddress = address;
	}

	void createOrder(User *user, Restaurant *rs, 
							   vector<MenuItem*>items, double totalPrice,
							   NotificationService *ns, PaymentStrategy *ps) override {
		this->id = ++nextId;
		this->user = user;
		this->rs = rs;
		this->items = items;
		this->totalPrice = totalPrice;
		this->ns = ns;
		this->type = "Pickup";
		this->ps = ps;
	}

	void displayOrder() override {
		cout<<"----------- ORDER'S DETAILS ----------------"<<endl;

		cout<<" Order's ID : "<< id << endl;
		cout<<" User's Name : "<< user->getName() << endl;
		cout<<" Restaurant's Name : "<< rs->getName() << endl;
		cout<<" Restaurant's Address : "<< rsAddress << endl;
		cout<<" Items Ordered : "<< endl;
		for(auto item : items){
			cout<<" - " << item->getName() << " : $" << item->getPrice() << endl;
		}
		cout<<" Total Price : $" << totalPrice << endl;
	}
};
	
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////// ORCHESTRATOR CLASS //////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////


class Zomato {
	RestaurantManager *rsInstance;
public:
	Zomato(){
		rsInstance = RestaurantManager::getInstance();

		MenuItem *dosa = new MenuItem("dosa", 150.0);
		MenuItem *idli = new MenuItem("idli", 15);
		MenuItem *samosa = new MenuItem("samosa", 25);

		Restaurant *haldiram = new Restaurant("Haldiram", "delhi");
		haldiram->addItem(dosa);
		haldiram->addItem(samosa);
		Restaurant *bikaner = new Restaurant("Bikaner", "delhi");
		bikaner->addItem(idli);
		bikaner->addItem(samosa);
		Restaurant *rameshwar = new Restaurant("Rameshwar", "Bangalore");
		rameshwar->addItem(dosa);
		rameshwar->addItem(idli);

		rsInstance->addRestaurants(haldiram);
		rsInstance->addRestaurants(bikaner);
		rsInstance->addRestaurants(rameshwar);
	}

	vector<Restaurant*> searchRestaurants(string &loc){
		vector<Restaurant*> restaurants = rsInstance->getRestaurants(loc);
		if(restaurants.empty()){
			cout<<"No restaurants found in this location."<<endl;
			return vector<Restaurant*>();
		}
		cout<<"Restaurants found in " << loc << " : "<<endl;
		for(auto res : restaurants){
			cout<<" - " << res->getName() << endl;
		}
		return restaurants;
	}

	void placeOrder(User *user, Restaurant *rs, vector<MenuItem*>items, 
					double totalPrice, NotificationService *ns, PaymentStrategy *ps, 
					string orderType) {
		Order *order;
		if(orderType == "Delivery") {
			order = new DeliveryOrder();
			static_cast<DeliveryOrder*>(order)->setAddress(user->getAddress());
		} else if(orderType == "Pickup") {
			order = new PickupOrder();
			static_cast<PickupOrder*>(order)->setRsAddress(rs->getAddress());
		} else {
			cout<<"Invalid order type."<<endl;
			return;
		}
		
		order->createOrder(user, rs, items, totalPrice, ns, ps);
		order->displayOrder();
		ps->pay();
		ns->notify();
	}

	void addToCart(User *user, Restaurant *rs, MenuItem *item) {
		user->addToCart(rs, item);
		cout<<"Item added to cart: " << item->getName() << endl;
	}

	void checkout(User *user, NotificationService *ns, PaymentStrategy *ps, string orderType) {
		Cart *cart = user->getCart();
		if(cart == nullptr) {
			cout<<"Cart is empty."<<endl;
			return;
		}

		vector<MenuItem*> items = cart->getItems();
		if(items.empty()) {
			cout<<"No items in the cart."<<endl;
			return;
		}

		double totalPrice = 0;
		for(auto item : items) {
			totalPrice += item->getPrice();
		}

		this->placeOrder(user, cart->getRestaurant(), items, totalPrice, ns, ps, orderType);
	}

	~Zomato() {
		delete rsInstance;
	}

};

int main(){
	cout<<"ONLINE FOOD ORDERING APPLICATION --> ZOMATO CLONE"<<endl;

	Zomato zomatoApp;
	string location = "delhi";
	User *user = new User("John Doe", "123 Main St, Delhi");

	vector<Restaurant*> resturant = zomatoApp.searchRestaurants(location);

	if(resturant.empty()) {
		cout<<"No restaurants found in this location."<<endl;
		return 0;
	}

	Restaurant *selectedRestaurant = resturant[0]; // Select the first restaurant for simplicity
	cout<<"Selected Restaurant: " << selectedRestaurant->getName() << endl;
	MenuItem *selectedItem = selectedRestaurant->getMenu()[0]; // Select the first item for simplicity
	cout<<"Selected Item: " << selectedItem->getName() << " - $" << selectedItem->getPrice() << endl;
	zomatoApp.addToCart(user, selectedRestaurant, selectedItem);


	selectedItem = selectedRestaurant->getMenu()[1];
	cout<<"Selected Item: " << selectedItem->getName() << " - $" << selectedItem->getPrice() << endl;
	zomatoApp.addToCart(user, selectedRestaurant, selectedItem);


	NotificationService *notificationService = new SMS(); // or new Email();
	PaymentStrategy *paymentStrategy = new UPI(); // or new CC(), NetBanking()
	zomatoApp.checkout(user, notificationService, paymentStrategy, "Delivery");
	delete user;


	return 0;
}
