#include "User.h"
#include <Order.h>
#include <sys/_types/_uuid_t.h>
#include <sys/types.h>
#include <uuid/uuid.h>

User::User(double cash): cash(cash) {
    uuid_generate(this->id);
}

std::string uuid_to_string(uuid_t uuid) {
    char str[37];
    uuid_unparse(uuid, str);
    return std::string(str);
}

double User::getBalance() {
    std::lock_guard<std::mutex> lock(mutex);
    return cash;
}

std::string User::get_id() {
    std::lock_guard<std::mutex> lock(mutex);
    return uuid_to_string(id);
}

std::unordered_map<std::string, std::vector<Order>> User::get_orders() {
    std::lock_guard<std::mutex> lock(mutex);
    return orders;
}

std::optional<Order> User::has_buy_order(std::string stock_symbol, double price, int quantity) {
    std::lock_guard<std::mutex> lock(mutex);
    for (const auto& order : orders[stock_symbol]) {
        if (order.price >= price && order.quantity == quantity && order.type == OrderType::BUY) {
            return order;
        }
    }
    return std::nullopt;
}

bool User::isSelling(std::string symbol, Order& order) {
    std::lock_guard<std::mutex> lock(mutex);
    auto it = orders.find(symbol);
    if (it == orders.end()) return false;
    for (const auto& o : it->second) {
        if (order.price == o.price && order.quantity == o.quantity && order.type == OrderType::SELL) {
            return true;
        }
    }
    return false;
}

void User::add_initial_shares(Stock& stock, int quantity) {
    std::lock_guard<std::mutex> lock(mutex);
    orders[stock.getSymbol()].push_back({ OrderType::BUY, stock.getPrice(), quantity });
}

void User::set_funds(double amount) {
    std::lock_guard<std::mutex> lock(mutex);
    cash += amount;
}
void User::deduct_funds(double amount) {
    std::lock_guard<std::mutex> lock(mutex);
    cash -= amount;
}

int User::buy_stock(
    Stock& stock,
    double price,
    int quantity
) {
    std::lock_guard<std::mutex> lock(mutex);

    std::string symbol = stock.getSymbol();
    
    double totalCost = price * quantity;
    // lets say totalCost is 100 and cash is 50
    if (this->cash < totalCost) {
        /// we cant make the transaction
        return -1;
    }
    /// we have the balance, so we can do the transaction
    orders[symbol].push_back({ OrderType::BUY, price, quantity });
    return 0;
}

int User::sell_stock(
    uuid_t order_id,
    double price,
    int quantity
) {
    std::lock_guard<std::mutex> lock(mutex);

    Order* order = nullptr;
    std::string orderSymbol;
    for (auto& [symbol, orderList] : orders) {
        for (auto it = orderList.begin(); it != orderList.end(); ++it) {
            
            if (uuid_to_string(it->id) == uuid_to_string(order_id)) {
                order = &(*it);
                orderSymbol = symbol;
                break;
            }
        }
    }
    if (order == nullptr) {
        return -1;
    }

    /// we have the order, so we store it as a sell, till someone else buys this stock
    orders[orderSymbol].push_back({ OrderType::SELL, price, quantity });
    return 0;
}
