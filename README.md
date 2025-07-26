# ecommerce-system

E-commerce System – Full Stack Development Internship Challenge

Task Description
This project is a console-based simulation of an e-commerce system developed in C++. It fulfills the following requirements as described in the internship challenge:

Features Implemented:
- Product Definition:
  - All products have: name, price, and quantity.
  - Products may be:
    - Expirable (e.g., Cheese, Biscuits)
    - Non-expirable (e.g., TV, Mobile Scratch Card)
    - Shippable (e.g., Cheese, TV) – must include weight
    - Non-shippable (e.g., Mobile Scratch Card)

- Cart Functionality:
  - Customers can add products to a cart with a specific quantity.
  - Quantity added must not exceed available stock.

- Checkout System:
  - Verifies:
    - No expired products
    - Enough product stock
    - Cart is not empty
    - Customer balance is sufficient
  - Prints:
    - Order subtotal
    - Shipping fees (10 units per shippable item)
    - Total amount (subtotal + shipping)
    - Customer's remaining balance

- Shipping Service:
  - Collects only shippable items.
  - Displays itemized shipping list and total weight.

Sample Use Case (Code):
cart.add(cheese, 2);
cart.add(biscuits, 1);
cart.add(scratchCard, 1);
checkout(customer, cart);

Console Output:
** Shipment notice **
Cheese 200g
Cheese 200g
Biscuits 700g
Total package weight 1.1kg
** Checkout receipt **
2x Cheese 200
1x Biscuits 150
1x Mobile Scratch Card 50
----------------------
Subtotal 400
Shipping 30
Amount 430
Customer balance: 70
END.

Assumptions:
- Shipping cost is 10 units per shippable item.
- Expired items are detected using time() logic.
- Customer balance is checked before any deduction or processing.
- ShippingService accepts a list of objects implementing:
  - std::string getName() const
  - double getWeight() const

How to Run:
1. Clone this repository:
   git clone https://github.com/yourusername/ecommerce-system.git
   cd ecommerce-system

2. Compile the C++ file:
   g++ -std=c++11 main.cpp -o ecommerce

3. Run the executable:
   ./ecommerce

Programming Language:
C++

Notes:

- All edge cases (out-of-stock, expired items, empty cart, low balance) are handled.
- Adheres strictly to the problem specification.

Author:
Developed by Rewan Hagag
For the Full Stack Development Internship Challenge
