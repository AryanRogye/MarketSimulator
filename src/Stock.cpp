#include "Stock.h"
#include <Order.h>
#include <mutex>

Stock::Stock(std::string symbol, double price, int shares) {
    this->symbol = symbol;
    this->price = price;
    this->shares = shares;
    this->currentTime = 0.0;
    
    this->history.push_back({ price, shares, 0.0 } );
    this->times.push_back(0.0);
    this->prices.push_back(price);
    
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
int Stock::getShares() const {
    std::lock_guard<std::mutex> lock(mutex);
    return this->shares;
}
int Stock::getAvailableShares() const {
    std::lock_guard<std::mutex> lock(mutex);
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

std::optional<Order> Stock::isSelling(double price, int quantity) {
    std::lock_guard<std::mutex> lock(mutex);
    return this->orderBook.isSelling(price, quantity);
}

void Stock::executeTrade(
    double price,
    int quantity,
    double time
) {
    std::lock_guard<std::mutex> lock(mutex);
    this->price = price;
    this->currentTime = time;
    
    this->times.push_back(time);
    this->prices.push_back(price);
    this->history.push_back(Trade {price, quantity, time});

    this->highestPrice = std::max(this->highestPrice, price);
    this->lowestPrice = std::min(this->lowestPrice, price);
    this->orderBook.addBuyOrder(price, quantity);
}

int Stock::buy_shares(double price, int quantity, double time) {
    std::lock_guard<std::mutex> lock(mutex);

    std::optional<Order> order = this->orderBook.isSelling(price, quantity);

    if (order.has_value()) {

        double executionPrice = order->price;

        /// Price is always set to the last most recently completed trade
        this->price = executionPrice;
        this->currentTime = time;
        
        this->times.push_back(time);
        this->prices.push_back(executionPrice);
        this->history.push_back(Trade {executionPrice, quantity, time });
        
        this->highestPrice = std::max(this->highestPrice, executionPrice);
        this->lowestPrice = std::min(this->lowestPrice, executionPrice);
        this->orderBook.addBuyOrder(executionPrice, quantity);
        return 0;
    }

    return -1;
}

int Stock::sell_shares(uuid_t order_id, double price, int quantity, double time) {
    std::lock_guard<std::mutex> lock(mutex);

    return orderBook.addSellOrder(order_id, price, quantity);
}

std::optional<Order> OrderBook::isSelling(double price, int quantity) {
    std::lock_guard<std::mutex> lock(mutex);
    for (auto& order : this->sell_orders) {
        if (price >= order.price && order.quantity == quantity) {
            return order;
        }
    }
    return std::nullopt;
}

int OrderBook::addBuyOrder(double price, int quantity) {
    std::lock_guard<std::mutex> lock(mutex);
    this->buy_orders.push_back(Order(OrderType::BUY, price, quantity));
    return 0;
}
int OrderBook::addSellOrder(uuid_t order_id, double price, int quantity) {
    std::lock_guard<std::mutex> lock(mutex);

    // make sure that the order_id is not already in the sell_orders
    // but is in the buy_orders
    for (auto& order : this->sell_orders) {
        /// this means the order_id is already in the sell_orders
        if (Order::ordersMatch(order.id, order_id)) {
            return -1;
        }
    }
    for (auto& order : this->buy_orders) {
        if (Order::ordersMatch(order.id, order_id)) {
            this->sell_orders.push_back(Order(OrderType::SELL, price, quantity));
            this->buy_orders.erase(std::remove(this->buy_orders.begin(), this->buy_orders.end(), order), this->buy_orders.end());
            return 0;
        }
    }

    /// should never happen
    return -1;
}
