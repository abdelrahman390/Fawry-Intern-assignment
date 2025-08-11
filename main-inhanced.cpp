#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <stdexcept>

using namespace std;
/*
abstract: product
---- shepable > weight
---- expairable > expaireMonth
*/

string weightCheck(int weight){
    if(weight > 1000){
        return to_string(weight / 1000) + "kg";
    } else {
        return to_string(weight) + "g";
    }
}

// Abstract class
class Product {
    float price;
    float quantity;
    string name;
    
    public :
        Product(string _name, float _price, float _quantity){
            name = _name;
            price = _price;
            quantity = _quantity;
        }

        virtual void viewDetail(){
            // cout << "--------------------------" << endl;
            cout << "Name: " << name << endl;
            cout << "Price: " << price << endl;
            cout << "Quantity: " << quantity << endl;
        }

        bool isAvailable(float _quantity){
            if(_quantity <= quantity){
                return 1;
            } else {
                return 0;
            }
        }

        float getPrice(){
            return price;
        }

        virtual string getName(){
            return name;
        }

        void removeQuantity(float qnt){
            quantity -= qnt;
        }
};

class shepable : public Product  {
    float weight;
    public :
        shepable(string _name, float _price, float _quantity, float _weight) : Product(_name, _price, _quantity){
            weight = _weight;
        }

        float getWeight(){
            return weight;
        }

        virtual void viewDetail(){
            Product::viewDetail();
            cout << "Weight: " << weight << endl;
        }
};

class expairable : public shepable   {
    int expaireMonth;

    public :
        expairable(string _name, float _price, float _quantity, float _weight, int _expaireMonth) : shepable(_name, _price, _quantity, _weight){
            expaireMonth = _expaireMonth;
        }

        bool expired(){
            time_t now = time(nullptr);     // current time
            tm* localTime = localtime(&now);

            int monthNumber = localTime->tm_mon + 1;
            if(expaireMonth > monthNumber){
                return 0;
            }
            return 1;
        }

        virtual void viewDetail(){
            shepable::viewDetail();
            cout << "ExpaireMonth: " << expaireMonth << endl;
            cout << "Expired: " << (expired() ? "Yes" : "No") << endl;
        }

};

class market {
    vector<Product*> products;

    public:
        market(){
            cout << "\nInter 0 for not shepable or expairable products, 1 for shepable products, and 2 for expairable products, then other product data" << endl;
        }

        Product* addProduct(int ch, string _name, float _price, float _quantity, float _weight = 0, int _expaireMonth = 0){
            Product* p = NULL;
            switch (ch) {
            case 0:
                p = new Product(_name, _price, _quantity);
                // p->viewDetail();
                products.push_back(p);
                return p;
                break;
            case 1:
                if(_weight == 0){
                    cout << "Invalid weight value" << endl;
                } else {
                    p = new shepable(_name, _price, _quantity, _weight);
                    // p->viewDetail();
                    products.push_back(p);
                }
                return p;
                // break;
            case 2:
                if(_weight == 0 || _expaireMonth == 0){
                    cout << "Invalid weight or expaireMonth value" << endl;
                } else {
                    p = new expairable(_name, _price, _quantity, _weight, _expaireMonth);
                    // p->viewDetail();
                    products.push_back(p);
                }
                return p;
                // break;
            default:
                cout << "Invalud product type choice" << endl;
                return p;
                // break;
            }
        }

};

class Cart{
    vector<pair<Product*, float>> addedProducts;
    vector<pair<shepable *, float>> shepableProducts;

    public:
        bool thereAreShepable;
        float totalPrice = 0;

        void add(Product *product, float quntity)
        {
            if (product->isAvailable(quntity))
            {
                pair<Product *, float> item = {product, quntity};
                addedProducts.push_back(item);
                
                // Check if shepable
                if(dynamic_cast<shepable*>(product)){
                    thereAreShepable = 1;
                    shepableProducts.push_back({dynamic_cast<shepable*>(product), quntity});
                }
                product->removeQuantity(quntity);
                totalPrice += (product->getPrice() * quntity);
            }
            else
            {
                throw invalid_argument("There is not enough quantity in the stock.");
            }
        }

        vector<pair<Product*, float>> getProductList(){
            return addedProducts;
        }

        vector<pair<shepable*, float>> getshepableProductsList(){
            return shepableProducts;
        }

        bool isImpty() {
            if(addedProducts.size() > 0){
                return 0;
            } else {
                return 1;
            }
        }

};

class customer{
    public:
    string name;
    float balance;
    customer(string _name, float _balance){
            name = _name;
            balance = _balance;
        }
};

class ShippingSecvice{
    // vector<pair<shepable *, float>> shepableProducts;
    float totalWight = 0;

public:
    ShippingSecvice(vector<pair<shepable *, float>> _shepableProducts)
    {
        // shepableProducts = _shepableProducts;
        shipProducts(_shepableProducts);
    }

    void shipProducts(vector<pair<shepable *, float>> shepableProducts){
        cout << "** Shipment notice **" << endl;
        for(auto& p: shepableProducts){
            cout << p.second << "X " << p.first->getName() << "    " << weightCheck(p.first->getWeight() * p.second * 1000) << endl;
            totalWight += p.first->getWeight() * p.second ;
        }
        cout << "Total package weight " << weightCheck(totalWight) << "\n\n";
    }

};

void checkout(customer* customer, Cart* cart) {
    vector<pair<Product*, float>> products = cart->getProductList();
    vector<pair<shepable *, float>> shepableProducts = cart->getshepableProductsList();
    float total = 0;

    if(cart->isImpty()){
        throw invalid_argument("Cart is empty.");
    }
    
    if(customer->balance < cart->totalPrice){
        throw invalid_argument("insufficient balance!");
    }

    if(cart->thereAreShepable){
        ShippingSecvice s(shepableProducts);
    }

    cout << "** Checkout receipt **" << endl;
    for(auto& p: products){
        cout << p.second << "X " << p.first->getName() << "    " << p.first->getPrice() * p.second << "$" << endl;
        total += p.first->getPrice() * p.second ;
    }
    cout << "------------------------" << "\n";
    cout << "Subtotal      " << total << "$" << "\n";
    if(cart->thereAreShepable){
        cout << "Shipping      " << 30 << "$"  << "\n";
        total += 30;
    }
    cout << "Amount        " << total << "$"  << "\n";

}

int main(){
    market myMarket;

    Product* mobileScratch = myMarket.addProduct(0, "mobileScratch", 30, 5); // normal product
    Product* tv = myMarket.addProduct(1, "Tv", 1000, 15, 20); // shepable product
    Product* cheese = myMarket.addProduct(2, "cheese", 2, 10, 15, 10); // shepable and expirble product 

    // mobileScratch->viewDetail();
    // cout << "-----------------"<< endl;
    // tv->viewDetail();
    // cout << "-----------------"<< endl;
    // cheese->viewDetail();
    // cout << "-----------------"<< endl;
    // ****************** print every product details ******************

    Cart cart;
    cart.add(mobileScratch, 4);
    cart.add(tv, 1);

    customer abdelrahman("abdelrahman", 2000);

    checkout(&abdelrahman, &cart);

    return 0;
}
