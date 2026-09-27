#ifndef STOCK_H
#define STOCK_H

#include <mutex>
#include <string>
#include <vector>

class Stock {
public:
    Stock(std::string symbol, double price);
    
    std::string getSymbol() const;
    double getPrice() const;
    double getCurrentTime() const;
    std::vector<double> getPrices() const;
    std::vector<double> getTimes() const;

    double getHighestPrice() const;
    double getLowestPrice() const;

    void setPrice(double price, double currentTime);
    
private:
    std::string symbol;
    /// this represents the current time
    double price;
    double currentTime;

    std::vector<double> prices;
    std::vector<double> times;

    double highestPrice;
    double lowestPrice;

    mutable std::mutex mutex;
};

#endif
