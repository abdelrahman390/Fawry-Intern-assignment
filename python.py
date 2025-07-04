from abc import ABC, abstractmethod
from typing import List
from collections import defaultdict

# Define the interface for shippable products
class ShippableProduct(ABC):
    # accept only str as required
    @abstractmethod
    def get_name(self) -> str:
        pass
    
    # accept only float as required
    @abstractmethod
    def get_weight(self) -> float:
        pass

class Product(ShippableProduct):
    def __init__(self, name, price, quantity, expirable=False, expired=False, shippable=False, weight=0.0):
        self.name = name
        self.price = price
        self.quantity = quantity
        self.expirable = expirable
        self.expired = expired
        self.shippable = shippable
        self.weight = weight
    
    # Implement interface methods
    def get_name(self) -> str:
        return self.name
    
    def get_weight(self) -> float:
        return self.weight

    def remove_sold_quantity(self, count):
        self.quantity -= count

# Shipping service that processes shippable items
class ShippingService:
    # We can pass the items as a dictionary to contain the product quantity, name and weight but I passed it as a list as required.
    def ship_product(self, items: List[ShippableProduct]):
        print("\nShipping Service: Preparing to ship the following items:")
        product_counter = defaultdict(int)
        product_weights = defaultdict(float)
        total_weight = 0
        for item in items:
            product_counter[item.get_name()] += 1 
            product_weights[item.get_name()] += item.get_weight()
        # print all shippable products 
        for key in product_counter.keys():
            weight_grams = product_weights[key]  * 1000
            total_weight += weight_grams
            if(weight_grams > 1000):
                print(f"  - {product_counter[key]}x {key} ({weight_grams/1000}kg)")
            else:
                print(f"  - {product_counter[key]}x {key} ({weight_grams:.0f}g)")
        # print total package and handle it to be user friendly
        if(total_weight > 1000):
            print(f"Total package weight {round(total_weight/1000, 1) }kg")
        else:
            print(f"Total package weight {int(total_weight)}g")
        print("Items will be shipped within two days.")

class Cart:
    def __init__(self):
        self.items: List[CartItem] = []

    def add(self, product: Product, quantity):
        # to handle special casses when user add same product many times
        total_items_ordered = quantity
        for item in self.items:
            if(item.product.name == product.name):
                total_items_ordered += item.quantity

        # check for availability
        if total_items_ordered > product.quantity:
            raise ValueError(f"Sorry, we did not added {product.name} to cart, there is only {product.quantity} {product.name} products in stock")
        
        # check for expire
        if product.expired:
            raise ValueError(f"Sorry, {product.name} is Expired")
        self.items.append(CartItem(product, quantity))

    def is_empty(self):
        return len(self.items) == 0

    def calc_subtotal(self):
        sum = 0
        for item in self.items:
            sum += item.get_total_price()
        return sum

    def shippable_items_check(self):
        for item in self.items:
            if(item.product.shippable):
                return True
        return False

    def clear(self):
        self.items.clear()

class CartItem:
    def __init__(self, product: Product, quantity):
        self.product = product
        self.quantity = quantity

    def get_total_price(self):
        return self.product.price * self.quantity

class Customer:
    def __init__(self, name, amount):
        self.name = name
        self.amount = amount


def checkout(customer: Customer, cart: Cart):
    if cart.is_empty():
        # Empty cart error
        raise ValueError("Sorry, Cart is empty.")

    subtotal = cart.calc_subtotal()
    shippable_items_found = cart.shippable_items_check()
    shipping_cost = 30 if shippable_items_found else 0
    total = subtotal + shipping_cost

    if customer.amount < total:
        # insufficient amount rror
        raise ValueError("Sorry, insufficient balance!.")

    print("** Checkout receipt **")
    for item in cart.items:
        # print all added products as required
        if item.product.weight != 0:
            product_total_weight = round(item.product.weight * item.quantity * 1000, 1)
            if(product_total_weight >= 1000):
                print(f"{item.quantity}x {item.product.name} {int(product_total_weight/1000)}kg")
            else:
                print(f"{item.quantity}x {item.product.name} {int(product_total_weight)}g")
        else:
            print(f"{item.quantity}x {item.product.name}")

    print("----------------------")
    print(f"Subtotal {subtotal}")
    print(f"Shipping {shipping_cost}")
    print(f"Amount {total}")
    # remove the total order price from the user amount
    customer.amount -= total
    print(f"Customer balance after payment: {customer.amount}")

    if(shippable_items_found):
        # add every shippable items in (shippable_items) array
        shippable_items = []
        for cart_item in cart.items:
            if cart_item.product.shippable:
                for _ in range(cart_item.quantity):
                    shippable_items.append(cart_item.product)

        # Sending to shipping service 
        shipping_service = ShippingService()
        shipping_service.ship_product(shippable_items)

    for item in cart.items:
        item.product.remove_sold_quantity(item.quantity)
    cart.clear()

# create products to test
cheese = Product("Cheese", 100, 5, expirable=True, expired=False, shippable=True, weight=0.2)
biscuits = Product("Biscuits", 150, 3, expirable=True, expired=False, shippable=True, weight=0.7)
tv = Product("TV", 300, 10, shippable=True, weight=3.0)
scratch_card = Product("Mobile Scratch Card", 50, 10, shippable=False)

# create our customer like Abdelrahman
abdelrahman = Customer("Abdelrahman", 2000)

# initialize cart
cart = Cart()

# add some products to cart

# special case and handled correctly
# cart.add(cheese, 2)
# cart.add(cheese, 2)
# cart.add(cheese, 2)

cart.add(cheese, 2)
cart.add(biscuits, 2)
cart.add(tv, 3)
cart.add(scratch_card, 1)

checkout(abdelrahman, cart)

