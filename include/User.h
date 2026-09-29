#ifndef USER_H
#define USER_H

#include "Order.h"
#include <Stock.h>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>
#include <uuid/uuid.h>

class User {
    public:
        User(double cash);
        
        double getBalance();
        std::unordered_map<std::string, std::vector<Order>> get_orders();
        void add_initial_shares(Stock& stock, int quantity);
        int buy_stock(
            Stock& stock,
            double price,
            int quantity
        );
        int sell_stock(
            uuid_t order_id,
            double price,
            int quantity
        );
        std::string get_id();
        void set_funds(double amount);
        void deduct_funds(double amount);
        std::optional<Order> has_buy_order(std::string stock_symbol, double price, int quantity);
        bool isSelling(std::string symbol, Order& order);
    private:
        uuid_t id;
        double cash;
        std::unordered_map<std::string, std::vector<Order>> orders;
        std::mutex mutex;
};

#endif // USER_H
