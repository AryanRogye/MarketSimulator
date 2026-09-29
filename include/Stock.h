#ifndef STOCK_H
#define STOCK_H

#include <Order.h>
#include <mutex>
#include <string>
#include <sys/_types/_uuid_t.h>
#include <optional>
#include <vector>

struct Trade {
    double price;
    int quantity;
    double time;
};

class OrderBook {
    public:
        std::optional<Order> isSelling(double price, int quantity);
        int addBuyOrder(double price, int quantity);
        int addSellOrder(uuid_t order_id, double price, int quantity);
    private:
        // buy orders
        std::vector<Order> buy_orders;
        // sell orders
        std::vector<Order> sell_orders;
        mutable std::mutex mutex;
};

class Stock {
public:
    Stock(std::string symbol, double price, int shares);
    
    std::string getSymbol() const;
    double getPrice() const;
    double getCurrentTime() const;
    int getShares() const;
    int getAvailableShares() const;
    std::vector<double> getPrices() const;
    std::vector<double> getTimes() const;

    double getHighestPrice() const;
    double getLowestPrice() const;

    void executeTrade(double price, int quantity, double time);
    int buy_shares(double price, int quantity, double time);
    int sell_shares(uuid_t order_id, double price, int quantity, double time);
    std::optional<Order> isSelling(double price, int quantity);
private:
    std::string symbol;

    /// A Stocks displayed/current price is the price of the most recent completed trade
    double price;

    /// This cannot change
    int shares;
    
    double currentTime;

    std::vector<Trade> history;
    std::vector<double> prices;
    std::vector<double> times;

    OrderBook orderBook;

    double highestPrice;
    double lowestPrice;

    mutable std::mutex mutex;
};

#endif
