// LLD PROBLEM -->
// ONLINE DELIVERY + INVENTORY MANAGEMENT

//////////////////////////////////////////////////////////////////
//          REQUIREMENTS 
//////////////////////////////////////////////////////////////////
/*

FUNCTIONAL REQUIREMENTS

- WE SHOULD BE ABLE TO MANAGE INVENTORY (ADD / REMOVE ITEMS).
- WE SHOULD HAVE REPLENISH STRATEGIES (THRESHOLD, WEEKLY) & SHOULD BE SCALABLE
- WE CAN HAVE MULTIPLE INVENTORYSTORE (LIKE, DBINVETORYSTORE, ETC) &
   CAN BE FURTHER EXTERNDED.
- WE SHOULD ABLE TO SEE ITEMS FROM ALL THE DARKSTORES CLOSER TO HIM / HER (5KM).
- IF ONE DARKSTORE CANNOT FULFIL THE ORDER, ONE ORDER CAN BE SPLIT INTO MULTIPLE
    DATASTORES, FULFILED BY MULTIPLE DELIVERY AGENT.

NON-FUNCTIONAL REQUIREMENTS

- SCALABLE
- PLUG N PLAY
- FOLLOW SOLID PRINCIPLES
===================================================================

PATTERNS USED TO SOLVE THIS PROBLEM

- STRATEGY
- FACTORY (FACTORY METHOD)
- SINGLETON
- BRIDGE
- FACADE
*/

//////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////////////////
//                PRODUCT
//////////////////////////////////////////////////////////////////

class Product {
private:
	string name;
	double price;
	int sku;
public:
	Product(int id, string name, double price){
		this->sku = id;
		this->name = name;
		this->price = price;
	}

	string getName() { return name;}
	double getPrice() {return price;}
	int getSku() {return sku;}
};

class ProductFactory {
public:
	static shared_ptr<Product> createProduct(int sku) {
		string name;
        double price;

        if (sku == 101) {
            name  = "Apple";
            price = 20;
        }
        else if (sku == 102) {
            name  = "Banana";
            price = 10;
        }
        else if (sku == 103) {
            name  = "Chocolate";
            price = 50;
        }
        else if (sku == 201) {
            name  = "T-Shirt";
            price = 500;
        }
        else if (sku == 202) {
            name  = "Jeans";
            price = 1000;
        }
        else {
            name  = "Item" + to_string(sku);
            price = 100;
        }

		return make_shared<Product>(sku, name, price);
	}
};


//////////////////////////////////////////////////////////////////
//         INVENTORY STORE AND DBINVENTORTY STORE
//////////////////////////////////////////////////////////////////

class InventoryStore {
public:
	virtual ~InventoryStore() {}
	virtual void addProduct(shared_ptr<Product> prod, int quantity) = 0;
	virtual void removeProduct(int sku, int quantity) = 0;
	virtual int checkStock(int sku) = 0;
	virtual vector<shared_ptr<Product>> listProduct() = 0;
};

class DbInventoryStore : public InventoryStore {
private:
	unordered_map<int,int> stocks;
	unordered_map<int, shared_ptr<Product>>products;

public:
	DbInventoryStore(){}

	void addProduct(shared_ptr<Product> prod, int qty) override {
		int sku = prod->getSku();
        if (products.count(sku) == 0) {
            products[sku] = prod;
        }
        stocks[sku] += qty;
	}

	void removeProduct(int sku, int qty) override {
		if(stocks.find(sku) == stocks.end()) return;

        int currentQuantity = stocks[sku];
        int remainingQuantity = currentQuantity - qty;
        if (remainingQuantity > 0) {
            stocks[sku] = remainingQuantity;
        } else {
            stocks.erase(sku);
        }
	}

	int checkStock(int sku) override {
		if(stocks.find(sku) == stocks.end()) return 0;

		return stocks[sku];
	}

	vector<shared_ptr<Product>> listProduct() override {
		vector<shared_ptr<Product>>available;

		for(auto it : stocks){
			int sku = it.first;
            int qty = it.second;
            if (qty > 0 && products.count(sku)) {
				available.push_back(products[sku]);
			}
		}

		return available;
	}
};


//////////////////////////////////////////////////////////////////
//              INVENTORYMANAGER 
/////////////////////////////////////////////////////////////////

class InventoryManager {
    shared_ptr<InventoryStore> store;

public:
    InventoryManager(shared_ptr<InventoryStore> store) {
        this->store = store;
    }

    void addStock(int sku, int qty) {
        shared_ptr<Product> prod = ProductFactory::createProduct(sku);
        store->addProduct(prod, qty);
        cout << "[InventoryManager] Added SKU " << sku << " Qty " << qty << endl;
    }

    void removeStock(int sku, int qty) {
        store->removeProduct(sku, qty); 
    }

    int checkStock(int sku) {
        return store->checkStock(sku);
    }

    vector<shared_ptr<Product>> getAvailableProducts() {
        return store->listProduct();
    }
};


//////////////////////////////////////////////////////////////////
//              REPLANISHMENT STRATEGY 
/////////////////////////////////////////////////////////////////

class ReplenishStrategy {
public:
	virtual void replenish(shared_ptr<InventoryManager> manager, unordered_map<int,int> itemToReplanish) = 0;
	virtual ~ReplenishStrategy() {}
};

class ThresholdReplenishStrategy : public ReplenishStrategy {
private:
	int threshold;

public:
	ThresholdReplenishStrategy(int threshold){
		this->threshold = threshold;
	}

	void replenish(shared_ptr<InventoryManager> manager, unordered_map<int,int> itemsToReplenish) override {
        cout << "[ThresholdReplenish] Checking threshold... \n";
        for (auto it : itemsToReplenish) {
            int sku = it.first;
            int qtyToAdd = it.second;
            int current  = manager->checkStock(sku);
            if (current < threshold) {
                manager->addStock(sku, qtyToAdd);
                cout << "  -> SKU " << sku << " was " << current 
                     << ", replenished by " << qtyToAdd << endl;
            }
        }
    }
    
};

class WeeklyReplenishStrategy : public ReplenishStrategy {
public:
    WeeklyReplenishStrategy() {}
    void replenish(shared_ptr<InventoryManager> manager, unordered_map<int,int> itemsToReplenish) override {
        cout << "[WeeklyReplenish] Weekly replenishment triggered for inventory.\n";
    }
};


//////////////////////////////////////////////////////////////////
//                DARK STORES 
/////////////////////////////////////////////////////////////////

class DarkStore {
	string name;
	double x,y;
	shared_ptr<InventoryManager> inventoryManager;
	shared_ptr<ReplenishStrategy> replenishStrategy;

public:

	DarkStore(string name, int x, int y){
		this->name = name;
		this->x = x;
		this->y = y;
		
		this->inventoryManager = make_shared<InventoryManager>(make_shared<DbInventoryStore>());
	}

	double distanceTo(int ux, int uy){
		return sqrt((x - ux)*(x - ux) + (y - uy)*(y - uy));
	}

	void runReplanishment(unordered_map<int,int> itemsToReplenish){
		if(replenishStrategy){
			replenishStrategy->replenish(inventoryManager, itemsToReplenish);
		}
	}

	vector<shared_ptr<Product>> getAllProducts() {
		return inventoryManager->getAvailableProducts();
	}

	int checkStock(int sku) {
        return inventoryManager->checkStock(sku);
    }

    void removeStock(int sku, int qty) {
        inventoryManager->removeStock(sku, qty); 
    }

    void addStock(int sku, int qty) {
        inventoryManager->addStock(sku, qty);
    }

    // Getters & Setters
    void setReplenishStrategy(shared_ptr<ReplenishStrategy> strategy) {
        this->replenishStrategy = strategy;
    }

    string getName() {
        return this->name;
    }

    double getXCoordinate() {
        return this->x;
    }

    double getYCoordinate() {
        return this->y;
    }

    shared_ptr<InventoryManager> getInventoryManager() {
        return this->inventoryManager;
    }
};

/////////////////////////////////////////////
// DarkStoreManager (Singleton)
/////////////////////////////////////////////

class DarkStoreManager {
private:
	vector<shared_ptr<DarkStore>> darkStores;
    static DarkStoreManager* instance;
    static mutex mtx;

    DarkStoreManager() {
        //darkStores.resize(0);
    }

public:
	static DarkStoreManager* getInstance() {
		if(instance == nullptr){
			lock_guard<mutex> lock(mtx);
			if(instance == nullptr) {
				instance = new DarkStoreManager();
			}
		}

		return instance;
	}


	void registerDarkStore(shared_ptr<DarkStore> ds){
		this->darkStores.push_back(ds);
	}

	vector<shared_ptr<DarkStore>> getNearbyDarkStores(double ux, double uy, double maxDistance) {
        vector<pair<double,shared_ptr<DarkStore>>> distList;
        for (auto ds : darkStores) {
            double d = ds->distanceTo(ux, uy);
            if (d <= maxDistance) {
                distList.push_back(make_pair(d, ds));
            }
        }
        sort(distList.begin(), distList.end(),
             [](auto &a, auto &b){ return a.first < b.first; });

        vector<shared_ptr<DarkStore>> result;
        for (auto &p : distList) {
            result.push_back(p.second);
        }
        return result;
    }
};

DarkStoreManager* DarkStoreManager::instance = nullptr;
mutex DarkStoreManager::mtx;

/////////////////////////////////////////////
// User & Cart
/////////////////////////////////////////////

class Cart {
public:
    vector<pair<shared_ptr<Product>,int>> items;  // (Product*, qty)

    void addItem(int sku, int qty) {
        shared_ptr<Product> prod = ProductFactory::createProduct(sku);
        items.push_back(make_pair(prod, qty));
        cout << "[Cart] Added SKU " << sku << " (" << prod->getName() 
             << ") x" << qty << endl;
    }

    double getTotal() {
        double sum = 0.0;
        for (auto &it : items) {
            sum += (it.first->getPrice() * it.second);
        }
        return sum;
    }

    vector<pair<shared_ptr<Product>,int>> getItems() {
        return items;
    }

};


class User {
public:
    string name;
    double x, y;
    shared_ptr<Cart> cart;  // User owns a cart

    User(string n, double x_coord, double y_coord) {
        name = n;
        x = x_coord;
        y = y_coord;
        cart = make_shared<Cart>();
    }

    shared_ptr<Cart> getCart() {
        return cart;
    }
};

/////////////////////////////////////////////
// DeliveryPartner
/////////////////////////////////////////////

class DeliveryPartner {
public:
    string name;
    DeliveryPartner(string n) {
        name = n;
    }
};

/////////////////////////////////////////////
// Order & OrderManager (Singleton)
/////////////////////////////////////////////

class Order {
public:
    static int nextId;
    int orderId;
    shared_ptr<User> user;
    vector<pair<shared_ptr<Product>,int>> items;     // (Product*, qty)
    vector<shared_ptr<DeliveryPartner>> partners;
    double totalAmount;

    Order(shared_ptr<User> u) {
        orderId = nextId++;
        user = u;
        totalAmount = 0.0;
    }
};

int Order::nextId = 1;

// Singleton
class OrderManager {
private:
    vector<shared_ptr<Order>> orders;
    static OrderManager* instance;
    static mutex mtx;

    OrderManager() {
        orders.resize(0);
    }

public:
    static OrderManager* getInstance() {
        if(instance == nullptr) {
        	lock_guard<mutex> lock(mtx);
        	if(instance == nullptr){
        		instance = new OrderManager();
        	}
        }
        return instance;
    }

    void placeOrder(shared_ptr<User> user, shared_ptr<Cart> cart) {
        cout << "\n[OrderManager] Placing Order for: " << user->name << "\n";

        // product --> Qty
        vector<pair<shared_ptr<Product>,int>> requestedItems = cart->getItems();
    
        // 1) Find nearby dark stores within 5 KM
        double maxDist = 5.0;
        vector<shared_ptr<DarkStore>> nearbyDarkStores = DarkStoreManager::getInstance()->getNearbyDarkStores(user->x, user->y, maxDist);
        
        if (nearbyDarkStores.empty()) {
            cout << "  No dark stores within 5 KM. Cannot fulfill order.\n";
            return;
        }
    
        // 2) Check if closest store has all items
        shared_ptr<DarkStore> firstStore = nearbyDarkStores.front();

        bool allInFirst = true;
        for (pair<shared_ptr<Product>,int>& item : requestedItems) {

            int sku = item.first->getSku();
            int qty = item.second;

            if (firstStore->checkStock(sku) < qty) {
                allInFirst = false;
                break;
            }
        }
    
        shared_ptr<Order> order = make_shared<Order>(user);

        // One delivery partner required...
        if (allInFirst) {

            cout << "  All items at: " << firstStore->getName() << "\n";

            // Remove the products from store
            for (pair<shared_ptr<Product>,int>& item : requestedItems) {
                int sku = item.first->getSku();
                int qty = item.second;
                firstStore->removeStock(sku, qty);
                order->items.push_back({ item.first, qty });
            }

            order->totalAmount = cart->getTotal();
            order->partners.push_back(make_shared<DeliveryPartner>("Partner1"));
            cout << "  Assigned Delivery Partner: Partner1\n";
        } 

        // Multiple delivery partners required
        else {
            cout << "  Splitting order across stores...\n";

            map<int,int> allItems; //SKU --> Qty

            for (pair<shared_ptr<Product>,int>& item : requestedItems) {
                allItems[item.first->getSku()] = item.second;
            }
    
            int partnerId = 1;
            for (shared_ptr<DarkStore> store : nearbyDarkStores) {

                // If allItems becomes empty, we break early (all SKUs have been assigned)
                if (allItems.empty()) break;

                cout << "   Checking: " << store->getName() << "\n";
    
                bool assigned = false;
                vector<int> toErase;
                
                for (auto& [sku, qtyNeeded] : allItems) {

                    int availableQty = store->checkStock(sku);
                    if (availableQty <= 0) continue;
    
                    //take whichever is smaller: available or qtyNeeded.
                    int takenQty = min(availableQty, qtyNeeded);
                    store->removeStock(sku, takenQty);

                    cout << "     " << store->getName() << " supplies SKU " << sku 
                         << " x" << takenQty << "\n";

                    order->items.push_back({ ProductFactory::createProduct(sku), takenQty });
    
                    // Adjust the Quantity
                    if (qtyNeeded > takenQty) {
                        allItems[sku] = qtyNeeded - takenQty;
                    } else {
                        toErase.push_back(sku);
                    }
                    assigned = true;
                }

                // After iterating all SKUs in allItems, we erase 
                // any fully‐satisfied SKUs from the allItems map.
                for (int sku : toErase) allItems.erase(sku);
    
                // If at least one SKU was taken from this store, we assign 
                // a new DeliveryPartner.
                if (assigned) {
                    string pname = "Partner" + to_string(partnerId++);
                    order->partners.push_back(make_shared<DeliveryPartner>(pname));
                    cout << "     Assigned: " << pname << " for " << store->getName() << "\n";
                }
            }
    
            //  if remaining still has entries, we print which SKUs/quantities could not be fulfilled.
            if (!allItems.empty()) {
                cout << "  Could not fulfill:\n";
                for (auto& [sku, qty] : allItems) {
                    cout << "    SKU " << sku << " x" << qty << "\n";
                }
            }

            // recompute order->totalAmount
            double sum = 0;
            for (auto& item : order->items) {
                sum += item.first->getPrice() * item.second;
            }
            order->totalAmount = sum;
        }
    
        // Printing Order Summary
        cout << "\n[OrderManager] Order #" << order->orderId << " Summary:\n";
        cout << "  User: " << user->name << "\n  Items:\n";
        for (auto& item : order->items) {
            cout << "    SKU " << item.first->getSku()
                 << " (" << item.first->getName() << ") x" << item.second
                 << " @ ₹" << item.first->getPrice() << "\n";
        }
        cout << "  Total: ₹" << order->totalAmount << "\n  Partners:\n";
        for (shared_ptr<DeliveryPartner>dp : order->partners) {
            cout << "    " << dp->name << "\n";
        }
        cout << endl;
    
        orders.push_back(order);   
    }    

    vector<shared_ptr<Order>> getAllOrders() {
        return orders;
    }
};

OrderManager* OrderManager::instance = nullptr;
mutex OrderManager::mtx;


/////////////////////////////////////////////
// Main(): High-Level Flow
/////////////////////////////////////////////
class ZeptoHelper {
public:
    static void showAllItems(shared_ptr<User> user) {
        cout << "\n[Zepto] All Available products within 5 KM for " << user->name << ":\n";

        DarkStoreManager* dsManager = DarkStoreManager::getInstance();
        vector<shared_ptr<DarkStore>> nearbyStores = dsManager->getNearbyDarkStores(user->x, user->y, 5.0);

        // Collect each SKU → price (so we only display each product once)
        map<int, double> skuToPrice;
        map<int, string> skuToName;

        for (shared_ptr<DarkStore> darkStore : nearbyStores) {
            vector<shared_ptr<Product>> products = darkStore->getAllProducts();

            for (shared_ptr<Product> product : products) {
                int sku = product->getSku();

                if (skuToPrice.count(sku) == 0) {
                    skuToPrice[sku] = product->getPrice();
                    skuToName[sku]  = product->getName();
                }
            }
        }

        for (auto& entry : skuToPrice) {
            int sku = entry.first;
            double price = entry.second;
            cout << "  SKU " << sku << " - " << skuToName[sku] << " @ ₹" << price << "\n";
        }
    }

    static void initialize() {
        auto dsManager = DarkStoreManager::getInstance();

        // DarkStore A.......
        shared_ptr<DarkStore> darkStoreA = make_shared<DarkStore>("DarkStoreA", 0.0, 0.0);
        darkStoreA->setReplenishStrategy(make_shared<ThresholdReplenishStrategy>(3));
 
        cout << "\nAdding stocks in DarkStoreA...." << endl;  
        darkStoreA->addStock(101, 5); // Apple
        darkStoreA->addStock(102, 2); // Banana

        // DarkStore B.......
        shared_ptr<DarkStore> darkStoreB = make_shared<DarkStore>("DarkStoreB", 4.0, 1.0);
        darkStoreB->setReplenishStrategy(make_shared<ThresholdReplenishStrategy>(3));

        cout << "\nAdding stocks in DarkStoreB...." << endl; 
        darkStoreB->addStock(101, 3); // Apple
        darkStoreB->addStock(103, 10); // Chocolate

        // DarkStore C.......
        shared_ptr<DarkStore> darkStoreC = make_shared<DarkStore>("DarkStoreC", 2.0, 3.0);
        darkStoreC->setReplenishStrategy(make_shared<ThresholdReplenishStrategy>(3));

        cout << "\nAdding stocks in DarkStoreC...." << endl; 
        darkStoreC->addStock(102, 5); // Banana
        darkStoreC->addStock(201, 7); // T-Shirt

        dsManager->registerDarkStore(darkStoreA);
        dsManager->registerDarkStore(darkStoreB);
        dsManager->registerDarkStore(darkStoreC);
    }
};

/////////////////////////////////////////////
// Main(): High-Level Flow
/////////////////////////////////////////////

int main() {

    // 1) Initialize.
    ZeptoHelper::initialize();

    // 2) A User comes on Platform
    shared_ptr<User> user = make_shared<User>("Abhisek", 1.0, 1.0);
    cout <<"\nUser with name " << user->name<< " comes on platform" << endl;

    // 3) Show all available items via Zepto
    ZeptoHelper::showAllItems(user);

    // 4) User adds items to cart (some not in a single store)
    cout<<"\nAdding items to cart\n";
    shared_ptr<Cart> cart = user->getCart();
    cart->addItem(101, 4);  // dsA has 5, dsB has 3 
    cart->addItem(102, 3);  // dsA has 2, dsC has 5
    cart->addItem(103, 2);  // dsB has 10

    // 5) Place Order
    OrderManager::getInstance()->placeOrder(user, user->cart);
    return 0;
}


