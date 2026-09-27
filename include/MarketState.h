#ifndef MARKETSTATE_H
#define MARKETSTATE_H

#include "Stock.h"
#include <User.h>
#include <memory>
#include <vector>

class MarketState {
    public:
        MarketState();
        const std::vector<std::unique_ptr<Stock>>& getStocks() const;
        const std::vector<std::unique_ptr<User>>& getUsers() const;

        int setStockPrice(const std::string& symbol, double price, double currentTime);
    private:
        std::vector<std::unique_ptr<User>> users;
        std::vector<std::unique_ptr<Stock>> stocks;
};

#endif // MARKETSTATE_H
