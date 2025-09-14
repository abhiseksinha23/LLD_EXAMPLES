// LLD PROBLEM -->
// COUPON DISCOUNT SYSTEM

//////////////////////////////////////////////////////////////////
//          REQUIREMENTS 
//////////////////////////////////////////////////////////////////
/*

FUNCTIONAL REQUIREMENTS

- WE CAN ADD NEW COUPONS AT RUNTIME
- BOTH CART LEVEL AND PRODUCT LEVEL DISCOUNT
- SUPPORT MULTIPLE COUPON TYPES (SEASONAL OFFER, LOYALITY DISCOUNT, BANKING DISCOUNT, ETC)
- SUPPORT FLAT, PERCENTAGE AND PERCENTAGE_WITH_CAP DISCOUNT
- ONE COUPON CAN OR CANNNOT BE APPLIED ON TOP OF OTHER COUPONS.
- THREAD SAFE


NON-FUNCTIONAL REQUIREMENTS

- SCALABLE
- PLUG N PLAY
- FOLLOW SOLID PRINCIPLES


===================================================================

PATTERNS USED TO SOLVE THIS PROBLEM

- STRATEGY
- FACTORY (FACTORY METHOD)
- SINGLETON
- TEMPLATE DESIGN
- CHAIN OF RESPONSIBILITY
*/

//////////////////////////////////////////////////////////////////

#include <bits/stdc++.h>
using namespace std;

//////////////////////////////////////////////////////////////////
//                DISCOUNTSTRATEGY
//////////////////////////////////////////////////////////////////

class IDiscountStrategy {
public:
    virtual double calculateDiscount(double amt) = 0;
    virtual ~IDiscountStrategy() {}
};


class FlatDiscountStrategy : public IDiscountStrategy {
private:
  double discount;

public:
  FlatDiscountStrategy(double dis){
    this->discount = dis;
  }

  double calculateDiscount(double amt) override {
    return min(amt, discount);
  }
};


class PercentageDiscountStrategy : public IDiscountStrategy {
private:
  double percentage;

public:
  PercentageDiscountStrategy(double per){
    this->percentage = per;
  }

  double calculateDiscount(double amt) override {
    return (percentage * amt) / 100.0;
  }
};


class PercentageWithCapStrategy : public IDiscountStrategy {
private:
  double percentage;
  double cap;

public:
  PercentageWithCapStrategy(double per, double cap){
    this->percentage = per;
    this->cap = cap;
  }

  double calculateDiscount(double amt) override {
    double dis = (percentage * amt) / 100.0;

    if(dis > cap)return cap;
    return dis;
  }
};


//////////////////////////////////////////////////////////////////
//                DISCOUNT STRATEGY FACTORY
//////////////////////////////////////////////////////////////////



enum class StrategyType {
 FLAT,
 PERCENTAGE,
 PERCENTAGEWITHCAP
};


class DiscountStrategyFactory {
private:
  static shared_ptr<DiscountStrategyFactory> instance;
  DiscountStrategyFactory(){}

public:

  static shared_ptr<DiscountStrategyFactory> getInstance() {
    if(instance == nullptr){
    //   instance = new DiscountStrategyFactory();
        static std::shared_ptr<DiscountStrategyFactory> instance(
                new DiscountStrategyFactory());
    }

    return instance;
  }

  shared_ptr<IDiscountStrategy> getDiscountStrategy(StrategyType type, double param1, double param2 = 0.0) {
      if(type == StrategyType::FLAT){
        return make_shared<FlatDiscountStrategy>(param1);
      }

      if(type == StrategyType::PERCENTAGE){
        return make_shared<PercentageDiscountStrategy>(param1);
      }

      if(type == StrategyType::PERCENTAGEWITHCAP){
        return make_shared<PercentageWithCapStrategy>(param1, param2);
      }

      return nullptr;
  }
};

shared_ptr<DiscountStrategyFactory> DiscountStrategyFactory::instance = nullptr;


//////////////////////////////////////////////////////////////////
//                CART
//////////////////////////////////////////////////////////////////

class Product {
private:
  string name;
  string category;
  double price;

public:
  Product(string name, string category, double price){
    this->name = name;
    this->category = category;
    this->price = price;
  }

  string getName(){
    return name;
  }

  string getCategory(){
    return category;
  }

  double getPrice(){
    return price;
  }
};


class CartItem {
private:
  shared_ptr<Product> product;
  int quantity;

public:

  CartItem(shared_ptr<Product>product, int qty){
    this->product = product;
    this->quantity = qty;
  }

  double itemTotal() {
        return product->getPrice() * quantity;
    }


  const shared_ptr<Product> getProduct() {
        return product;
    }
};


class Cart {
private:
  vector<shared_ptr<CartItem>>items;
  string paymentBank;
  double originalTotal;
  double currentTotal;
  bool loyaltyMember;

public:
  Cart() {
      originalTotal = 0.0;
      currentTotal = 0.0;
      loyaltyMember = false;
      paymentBank = "";
    }

  void addProduct(shared_ptr<Product> prod, int qty = 1) {
      shared_ptr<CartItem> item = make_shared<CartItem>(prod, qty);
      items.push_back(item);
      originalTotal += item->itemTotal();
      currentTotal  += item->itemTotal();
  }

  double getOriginalTotal() {
      return originalTotal;
  }

  double getCurrentTotal() {
      return currentTotal;
  }

  void applyDiscount(double d) {
      currentTotal -= d;
      if (currentTotal < 0) {
          currentTotal = 0;
      }
  }

  void setLoyaltyMember(bool member) {
      loyaltyMember = member;
  }

  bool isLoyaltyMember() {
      return loyaltyMember;
  }

  void setPaymentBank(string bank) {
      paymentBank = bank;
  }

  string getPaymentBank() {
      return paymentBank;
  }

  vector<shared_ptr<CartItem>> getItems() {
      return items;
  }
};


// ----------------------------
// Coupon base class (Chain of Responsibility)
// ----------------------------

class ICoupon {
private:
  shared_ptr<ICoupon> next;

protected:
  shared_ptr<IDiscountStrategy> discountStrategy;

public:
 ICoupon(){
  next = nullptr;
 }

 void setNext(shared_ptr<ICoupon>next){
  this->next = next;
 }

 shared_ptr<ICoupon> getNext() {
  return next;
 }

 void applyDiscount(shared_ptr<Cart> cart){
    if (isApplicable(cart)) {
        double discount = getDiscount(cart);
        cart->applyDiscount(discount);
        cout << name() << " applied: " << discount << endl;
        if (!isCombinable()) {
            return;
        }
    }
    if (next) {
        next->applyDiscount(cart);
    }
 }

 virtual bool isApplicable(shared_ptr<Cart> cart) = 0;
 virtual double getDiscount(shared_ptr<Cart> cart) = 0;
 virtual bool isCombinable() {
    return true;
 }
 virtual string name() = 0;
};



class SeasonalCoupon : public ICoupon {
private:
    string category;
    double percentage;

public:
    SeasonalCoupon(string cat, double per){
      this->category = cat;
      this->percentage = per;
      this->discountStrategy = DiscountStrategyFactory::getInstance()->getDiscountStrategy(StrategyType::PERCENTAGE, per);
    }

    bool isApplicable(shared_ptr<Cart> cart) override {
        for(shared_ptr<CartItem> item : cart->getItems()){
          if(item->getProduct()->getCategory() == category){
            return true;
          }
        }

        return false;
    }


    double getDiscount(shared_ptr<Cart> cart) override {
        double subtotal = 0.0;
        for(shared_ptr<CartItem> item : cart->getItems()){
          if(item->getProduct()->getCategory() == category){
             subtotal += item->itemTotal();
          }
        }

        return discountStrategy->calculateDiscount(subtotal);
    }


    bool isCombinable() override {
        return true;
    }
    string name() override {
        return "Seasonal Offer " + to_string((int)percentage) + " % off " + category;
    }
};


class LoyaltyDiscount : public ICoupon {
private:
    double percent;
public:
    LoyaltyDiscount(double pct) {
        percent = pct;
        discountStrategy = DiscountStrategyFactory::getInstance()->getDiscountStrategy(StrategyType::PERCENTAGE, percent);
    }

    bool isApplicable(shared_ptr<Cart> cart) override {
        return cart->isLoyaltyMember();
    }
    double getDiscount(shared_ptr<Cart> cart) override {
        return discountStrategy->calculateDiscount(cart->getCurrentTotal());
    }
    string name() override {
        return "Loyalty Discount " + to_string((int)percent) + "% off";
    }
};


class BulkPurchaseDiscount : public ICoupon {
private:
    double threshold;
    double flatOff;
public:
    BulkPurchaseDiscount(double thr, double off) {
        threshold = thr;
        flatOff = off;
        discountStrategy = DiscountStrategyFactory::getInstance()->getDiscountStrategy(StrategyType::FLAT, flatOff);
    }
    
    bool isApplicable(shared_ptr<Cart> cart) override {
        return cart->getOriginalTotal() >= threshold;
    }
    double getDiscount(shared_ptr<Cart> cart) override {
        return discountStrategy->calculateDiscount(cart->getCurrentTotal());
    }
    string name() override {
        return "Bulk Purchase Rs " + to_string((int)flatOff) + " off over "
             + to_string((int)threshold);
    }
};

class BankingCoupon : public ICoupon {
private:
    string bank;
    double minSpend;
    double percent;
    double offCap;
public:
    BankingCoupon(const string& b, double ms, double percent, double offCap) {
        bank = b;
        minSpend = ms;
        this->percent = percent;
        this->offCap = offCap;
        discountStrategy = DiscountStrategyFactory::getInstance()->getDiscountStrategy(StrategyType::PERCENTAGEWITHCAP, percent, offCap);
    }

    bool isApplicable(shared_ptr<Cart> cart) override {
        return cart->getPaymentBank() == bank
            && cart->getOriginalTotal() >= minSpend;
    }
    double getDiscount(shared_ptr<Cart> cart) override {
        return discountStrategy-> calculateDiscount(cart->getCurrentTotal());
    }
    string name() override {
        return bank + " Bank percentage " + to_string((int)percent) + " off upto " + to_string((int) offCap);
    }
};


//////////////////////////////////////////////////////////////////
//                COUPON MANAGER
//////////////////////////////////////////////////////////////////

class CouponManager {
private:
    static CouponManager *instance;
    shared_ptr<ICoupon> head;
    mutable mutex mtx;
    CouponManager() {
        head = nullptr;
    }
public:
    static CouponManager* getInstance() {
        if (!instance) {
            instance = new CouponManager();
        }
        return instance;
    }

    void registerCoupon(shared_ptr<ICoupon> coupon) {
        lock_guard<mutex> lock(mtx);
        if (!head) {
            head = coupon;
        } else {
            shared_ptr<ICoupon> cur = head;
            while (cur->getNext()) {
                cur = cur->getNext();
            }
            cur->setNext(coupon);
        }
    }

    vector<string> getApplicable(shared_ptr<Cart> cart) const {
        lock_guard<mutex> lock(mtx);
        vector<string> res;
        shared_ptr<ICoupon> cur = head;
        while (cur) {
            if (cur->isApplicable(cart)) {
                res.push_back(cur->name());
            }
            cur = cur->getNext();
        }
        return res;
    }

    double applyAll(shared_ptr<Cart> cart) {
        lock_guard<mutex> lock(mtx);
        if (head) {
            head->applyDiscount(cart);
        }
        return cart->getCurrentTotal();
    }
};

CouponManager* CouponManager::instance = nullptr;


//////////////////////////////////////////////////////////////////
//                MAIN / CLIENT
//////////////////////////////////////////////////////////////////

int main(){
    CouponManager *mgr = CouponManager::getInstance();

    // shared_ptr<CouponManager> mgr = CouponManager::getInstance(); NOT WORKING FOR SINGLETON
    mgr->registerCoupon(make_shared<SeasonalCoupon>("Clothing", 10));
    mgr->registerCoupon(make_shared<LoyaltyDiscount>(5));
    mgr->registerCoupon(make_shared<BulkPurchaseDiscount>(1000, 100));
    mgr->registerCoupon(make_shared<BankingCoupon>("ABC", 2000, 15, 500));

    shared_ptr<Product> p1 = make_shared<Product>("Winter Jacket", "Clothing", (double)1000.0);
    shared_ptr<Product> p2 = make_shared<Product>("Smartphone", "Electronics", (double)20000.0);
    shared_ptr<Product> p3 = make_shared<Product>("Jeans", "Clothing", (double)1000.0);
    shared_ptr<Product> p4 = make_shared<Product>("Headphones", "Electronics", (double)2000.0);


    shared_ptr<Cart> cart = make_shared<Cart>();
    cart->addProduct(p1, 1);
    cart->addProduct(p2, 1);
    cart->addProduct(p3, 2);
    cart->addProduct(p4, 1);
    cart->setLoyaltyMember(true);
    cart->setPaymentBank("ABC");

    cout << "Original Cart Total: Rs " << cart->getOriginalTotal() << endl;
    cout << endl;

    vector<string> applicable = mgr->getApplicable(cart);
    cout << "Applicable Coupons:" << endl;
    for (string name : applicable) {
        cout << " - " << name << endl;
    }
    
     cout << endl;

    double finalTotal = mgr->applyAll(cart);
    cout << "Final Cart Total after discounts: " << finalTotal << " Rs" << endl;
    cout << endl;
 
    return 0;
}
