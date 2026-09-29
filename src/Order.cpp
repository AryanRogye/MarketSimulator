#include "Order.h"

Order::Order(OrderType type, double price, int quantity): type(type), price(price), quantity(quantity) {
    uuid_generate(this->id);
}
