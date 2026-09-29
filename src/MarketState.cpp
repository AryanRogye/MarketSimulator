#include <MarketState.h>
#include <Order.h>
#include <Stock.h>
#include <User.h>
#include <atomic>
#include <cstddef>
#include <iterator>
#include <memory>
#include <optional>
#include <random>
#include <set>
#include <unordered_map>
#include <utility>
#include <iostream>

// Splits `total` into `n` uneven parts that sum exactly to `total`.
// minEach guarantees every user gets at least that many shares.
std::vector<int> splitUneven(int total, size_t n, int minEach, std::mt19937& gen) {
    std::vector<int> result(n, minEach);
    if (n == 0) return result;

    int pool = total - static_cast<int>(n) * minEach;
    if (pool <= 0) return result; // not enough shares to go around

    std::uniform_int_distribution<int> distr(0, pool);

    std::vector<int> cuts;
    cuts.reserve(n + 1);
    cuts.push_back(0);
    for (size_t i = 0; i < n - 1; ++i) cuts.push_back(distr(gen));
    cuts.push_back(pool);
    std::sort(cuts.begin(), cuts.end());

    for (size_t i = 0; i < n; ++i) result[i] += cuts[i + 1] - cuts[i];
    return result;
}

/// should only be called once, at the start of the simulation
void MarketState::distributeShares() {
    std::mt19937 gen(std::random_device{}());   
    
    for (auto& stock: stocks) {
        int totalShares = stock.second->getShares();
        std::vector<int> split = splitUneven(totalShares, users.size(), 1, gen);

        for (size_t i = 0; i < users.size(); ++i) {
            users[i]->add_initial_shares(*stock.second, split[i]);
        }
    }
}

/// Simulation Buying
void MarketState::buyRandomOrder(User& user, double time) {
    if (stocks.empty()) return;
    
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> movement(5, 15);
    std::uniform_int_distribution<std::size_t> stockIndex(0, stocks.size() - 1);

    int stocksToBuy = movement(generator);
    for (int i = 0; i < stocksToBuy; ++i) {
        auto it = std::next(stocks.begin(), static_cast<int>(stockIndex(generator)));
        
        Stock& stock = *it->second;

        std::uniform_int_distribution<int> priceDistribution(-5, 5);
        double price = stock.getPrice() + priceDistribution(generator);
        
        this->buy_stock(stock, user, price, 1, time);
    }
}

/// Simulation Selling
void MarketState::sellRandomOrder(User& user, double time) {
    
    std::unordered_map<std::string, std::vector<Order>> userOrders = user.get_orders();
    
    std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<std::size_t> stockIndex(0, userOrders.size() - 1);

    bool didSell = false;
    std::set<int> checkedIndices;
    while (!didSell) {

        /// we check to see if we checked all stocks, if we did then we break
        if (checkedIndices.size() == userOrders.size()) break;
        
        /// get a index and check to see if we already checked, if we did
        /// then try again
        int index = static_cast<int>(stockIndex(generator));
        if (checkedIndices.count(index) > 0) continue;

        /// Add to the set of checked indices
        checkedIndices.insert(index);
        
        auto it = std::next(userOrders.begin(), static_cast<int>(stockIndex(generator)));

        /// we want to check if any of the orders contains a buy
        bool containsBuy = false;
        Order orderToSell = Order(OrderType::SELL, 0.0, 0.0);
        for (const auto& order: it->second) {
            if (order.type == OrderType::BUY) {
                containsBuy = true;
                orderToSell = order;
                break;
            }
        }

        /// if no buy order was found, then we skip this stock
        if (!containsBuy) continue;

        /// if a buy order was found, then we sell the stock
        /// we'll increase the price by 1 and sell the stock
        orderToSell.price += 1.0;
        this->sell_stock(
            orderToSell.id, 
            user, 
            it->first,
            orderToSell.price, 
            orderToSell.quantity,
            time
        );
    }
}

MarketState::MarketState() {
    this->stocks["AAPL"] = std::make_unique<Stock>("AAPL", 180, 100);
    this->stocks["MSFT"] = std::make_unique<Stock>("MSFT", 420.00, 100);
    this->stocks["NVDA"] = std::make_unique<Stock>("NVDA", 150.00, 100);
    
    std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> movement(-500, 500);
    
    /// we'll create fake users
    for (int i = 0; i < 10; ++i) {
        double cash = 1000.0 + movement(generator);
        users.push_back(std::make_unique<User>(cash));
    }
    this->distributeShares();
}

const std::unordered_map<std::string, std::unique_ptr<Stock>>& MarketState::getStocks() const {
    return this->stocks;
}
const std::vector<std::unique_ptr<User>>& MarketState::getUsers() const {
    return this->users;
}


void MarketState::executeTrade(
    Stock& stock,
    User& user,
    User& seller,
    Order& order,
    double time
) {
    stock.executeTrade(order.price, order.quantity, time);
    user.deduct_funds(order.price * order.quantity);
    seller.set_funds(order.price * order.quantity);
}

int MarketState::buy_stock(Stock& stock, User& user, double price, int quantity, double time) {
    if (user.buy_stock(stock, price, quantity) < 0) {
        return -1;
    }
    for (auto& stockPair : this->stocks) {
        if (stockPair.second->getSymbol() == stock.getSymbol()) {
            if (stockPair.second->buy_shares(
                price, 
                quantity, 
                time
            ) < 0) {
                return -1;
            }
            /// we can create the transaction because there is someone selling shares
            user.set_funds(price * quantity);
            break;
        }
    }
    return 0;
}

int MarketState::sell_stock(
    uuid_t order_id, 
    User& user,
    std::string symbol,
    double price,
    int quantity,
    double time
) {
    if (user.sell_stock(order_id, price, quantity) < 0) {
        return -1;
    }

    for (auto& stockPair : this->stocks) {
        if (stockPair.second->getSymbol() != symbol) continue;

        for (auto& buyer : users) {
            std::optional<Order> order = buyer->has_buy_order(symbol, price, quantity);
            if (!order.has_value()) continue;

            executeTrade(
                *stockPair.second, 
                *buyer,
                user,
                *order,
                time
            );
        }
        
        if (stockPair.second->sell_shares(order_id, price, quantity, time) < 0) {
            return -1;
        }
    }
    return 0;
}
