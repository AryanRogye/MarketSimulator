#include "Stock.h"
#include <mutex>

Stock::Stock(std::string symbol, double price) {
    this->symbol = symbol;
    this->price = price;
    this->currentTime = 0.0;
    this->prices.push_back(price);
    this->times.push_back(0.0);
    
    this->highestPrice = price;
    this->lowestPrice = price;
}

std::string Stock::getSymbol() const {
    std::lock_guard<std::mutex> lock(mutex);
    return this->symbol;
}
double Stock::getPrice() const {
    std::lock_guard<std::mutex> lock(mutex);
    return this->price;
}
double Stock::getCurrentTime() const {
    std::lock_guard<std::mutex> lock(mutex);
    return this->currentTime;
}
std::vector<double> Stock::getPrices() const {
    std::lock_guard<std::mutex> lock(mutex);
    return this->prices;
}
std::vector<double> Stock::getTimes() const {
    std::lock_guard<std::mutex> lock(mutex);
    return this->times;
}
double Stock::getHighestPrice() const {
    std::lock_guard<std::mutex> lock(mutex);
    return this->highestPrice;
}
double Stock::getLowestPrice() const {
    std::lock_guard<std::mutex> lock(mutex);
    return this->lowestPrice;
}

void Stock::setPrice(double price, double currentTime) {
    std::lock_guard<std::mutex> lock(mutex);
    this->price = price;
    this->currentTime = currentTime;
    this->prices.push_back(price);
    this->highestPrice = std::max(this->highestPrice, price);
    this->lowestPrice = std::min(this->lowestPrice, price);
    this->times.push_back(currentTime);
}
