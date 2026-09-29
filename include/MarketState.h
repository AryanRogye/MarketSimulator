#ifndef MARKETSTATE_H
#define MARKETSTATE_H

#include "Stock.h"
#include <Order.h>
#include <User.h>
#include <memory>
#include <unordered_map>
#include <vector>

class MarketState {
    public:
        MarketState();
        const std::unordered_map<std::string, std::unique_ptr<Stock>>& getStocks() const;
        const std::vector<std::unique_ptr<User>>& getUsers() const;

        void buyRandomOrder(User& user, double time);
        void sellRandomOrder(User& user, double time);
    private:
        std::vector<std::unique_ptr<User>> users;
        /// represents all the stocks in the market, with their shares
        std::unordered_map<std::string, std::unique_ptr<Stock>> stocks;

        void executeTrade(
            Stock& stock,
            User& user,
            User& seller,
            Order& order,
            double time
        );
        
        int buy_stock(
            Stock& stock, 
            User& user, 
            double price,
            int quantity,
            double time
        );
        int sell_stock(
            uuid_t order_id, 
            User& user,
            std::string symbol,
            double price,
            int quantity,
            double time
        );
        
        void distributeShares();
};

#endif // MARKETSTATE_H
