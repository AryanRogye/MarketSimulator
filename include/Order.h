#ifndef ORDER_H
#define ORDER_H

#include <sys/_types/_uuid_t.h>
#include <uuid/uuid.h>

enum OrderType {
    SELL,
    BUY,
};

struct Order {
    uuid_t id;
    OrderType type;
    double price; 
    int quantity;

    Order(OrderType type, double price, int quantity);
    bool operator==(const Order& other) const {
        return uuid_compare(this->id, other.id) == 0;
    }

    static inline bool ordersMatch(uuid_t id1, uuid_t id2) {
        return uuid_compare(id1, id2) == 0;
    }
};

#endif // ORDER_H
