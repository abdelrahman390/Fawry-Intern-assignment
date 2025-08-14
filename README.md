                                                 ┌──────────────────────┐
                                                 │      Product         │  (base class)
                                                 │ - name: string       │
                                                 │ - price: float       │
                                                 │ - quantity: float    │
                                                 │ # weight: float      │
                                                 │ + viewDetail()       │
                                                 │ + isAvailable(q)     │
                                                 │ + getPrice()         │
                                                 │ + getName()          │
                                                 │ + removeQuantity(q)  │
                                                 └─────────┬────────────┘
                                                           │ (virtual inheritance)
                                            ┌──────────────┴──────────────┐
                                            │                             │
                                    ┌───────┴────────┐           ┌────────┴────────┐
                                    │   shippable    │           │   expirable     │
                      (Interface)   │(inherits v-    │           │(inherits v-     │
                                    │  Product)      │           │  Product)       │
                                    │ + getWeight()  │           │ - expaireMonth  │
                                    │                │           │ + isExpired()   │
                                    └───────┬────────┘           │ + viewDetail()  │
                                            │                    └────────┬────────┘
                                            │                             │
                                            └───────────┬─────────────────┘
                                                        │
                                         ┌──────────────┴──────────────┐
                                         │ shippableAndexpirable       │  (multiple inheritance)
                                         │ inherits shippable,expirable│
                                         │ + viewDetail()              │
                                         └─────────────────────────────┘

Other cooperating classes (not part of product inheritance):
 - market          : manages product creation & container(vector<Product*>)
 - Cart            : holds vector<pair<Product*, quantity>> and vector<pair<shippable*, quantity>>
 - customer        : holds name & balance
 - ShippingSecvice : computes & prints shipment / total weight
 - free functions  : weightCheck(), checkoutReceipt(), checkout()
