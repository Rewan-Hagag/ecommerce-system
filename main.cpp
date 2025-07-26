#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <ctime>
#include <iomanip>

using namespace std;

// ---------------------- Interfaces ----------------------
class IShippable {
public:
    virtual string getName() const = 0;
    virtual double getWeight() const = 0;
    virtual ~IShippable() = default;
};

// ---------------------- Product Classes ----------------------
class Product {
protected:
    string name;
    double price;
    int quantity;
public:
    Product(string n, double p, int q) : name(n), price(p), quantity(q) {}
    virtual ~Product() {}

    virtual bool isExpired() const { return false; }
    virtual bool requiresShipping() const { return false; }

    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }

    void decreaseQuantity(int q) { quantity -= q; }
};

class ExpirableProduct : public Product {
    time_t expiryDate;
public:
    ExpirableProduct(string n, double p, int q, time_t expiry)
        : Product(n, p, q), expiryDate(expiry) {}

    bool isExpired() const override {
        return time(nullptr) > expiryDate;
    }
};

class ShippableProduct : public Product, public IShippable {
    double weight;
public:
    ShippableProduct(string n, double p, int q, double w)
        : Product(n, p, q), weight(w) {}

    bool requiresShipping() const override { return true; }
    double getWeight() const override { return weight; }
    string getName() const override { return name; }
};

class ExpirableShippableProduct : public ExpirableProduct, public IShippable {
    double weight;
public:
    ExpirableShippableProduct(string n, double p, int q, time_t expiry, double w)
        : ExpirableProduct(n, p, q, expiry), weight(w) {}

    bool requiresShipping() const override { return true; }
    double getWeight() const override { return weight; }
    string getName() const override { return name; }
};

// ---------------------- Cart and Checkout ----------------------
struct CartItem {
    shared_ptr<Product> product;
    int quantity;
};

class Cart {
    vector<CartItem> items;
public:
    void add(shared_ptr<Product> product, int quantity) {
        if (quantity > product->getQuantity())
            throw runtime_error("Cannot add " + to_string(quantity) + " of " + product->getName() + ": exceeds available stock.");
        product->decreaseQuantity(quantity);
        items.push_back({product, quantity});
    }

    const vector<CartItem>& getItems() const { return items; }

    bool isEmpty() const { return items.empty(); }
};

class Customer {
    string name;
    double balance;
public:
    Customer(string n, double b) : name(n), balance(b) {}

    string getName() const { return name; }
    double getBalance() const { return balance; }
    void deduct(double amount) { balance -= amount; }
};

class ShippingService {
public:
    static void shipItems(const vector<shared_ptr<IShippable>>& items) {
        cout << "** Shipment notice **\n";
        double totalWeight = 0;
        for (const auto& item : items) {
            cout << item->getName() << " " << item->getWeight() * 1000 << "g\n";
            totalWeight += item->getWeight();
        }
        cout << "Total package weight " << fixed << setprecision(1) << totalWeight << "kg\n";
    }
};

void checkout(Customer& customer, Cart& cart) {
    if (cart.isEmpty()) throw runtime_error("Cart is empty.");

    double subtotal = 0;
    double shipping = 0;
    vector<shared_ptr<IShippable>> toShip;

    for (const auto& item : cart.getItems()) {
        auto prod = item.product;
        if (prod->isExpired()) throw runtime_error(prod->getName() + " is expired.");
        subtotal += prod->getPrice() * item.quantity;

        if (prod->requiresShipping()) {
            shared_ptr<IShippable> ship = dynamic_pointer_cast<IShippable>(prod);
            for (int i = 0; i < item.quantity; ++i)
                toShip.push_back(ship);
            shipping += 10 * item.quantity; // Assuming a flat shipping fee per item
        }
    }

    double total = subtotal + shipping;
    if (customer.getBalance() < total) throw runtime_error("Insufficient balance for the total amount of " + to_string(total) + ".");

    ShippingService::shipItems(toShip);

    cout << "** Checkout receipt **\n";
    for (const auto& item : cart.getItems())
        cout << item.quantity << "x " << item.product->getName() << " " << item.product->getPrice() * item.quantity << "\n";
    cout << "----------------------\n";
    cout << "Subtotal " << subtotal << "\n";
    cout << "Shipping " << shipping << "\n";
    cout << "Total Amount " << total << "\n";

    customer.deduct(total);
    cout << "Customer balance after payment: " << customer.getBalance() << "\nEND.\n";
}

// ---------------------- Main Testing ----------------------
int main() {
    time_t future = time(nullptr) + 86400; // 1 day ahead

    auto cheese = make_shared<ExpirableShippableProduct>("Cheese", 100, 5, future, 0.2);
    auto biscuits = make_shared<ExpirableShippableProduct>("Biscuits", 150, 2, future, 0.7);
    auto tv = make_shared<ShippableProduct>("TV", 300, 3, 5.0);
    auto scratchCard = make_shared<Product>("Mobile Scratch Card", 50, 10);

    Customer customer("Alice", 500);
    Cart cart;
    try {
        cart.add(cheese, 2);
        cart.add(biscuits, 1);
        cart.add(scratchCard, 1);
        checkout(customer, cart);
    } catch (exception& e) {
        cout << "ERROR: " << e.what() << endl;
    }

    return 0;
}
