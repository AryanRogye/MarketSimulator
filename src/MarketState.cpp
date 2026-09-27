#include <MarketState.h>
#include <random>

MarketState::MarketState() {
    this->stocks.push_back(std::make_unique<Stock>("AAPL", 180.00));
    this->stocks.push_back(std::make_unique<Stock>("MSFT", 420.00));
    this->stocks.push_back(std::make_unique<Stock>("NVDA", 150.00));
    
    
    std::mt19937 generator(std::random_device{}());
    std::uniform_real_distribution<double> movement(-500, 500);
    
    /// we'll create fake users
    for (int i = 0; i < 10; ++i) {
        double cash = 1000.0 + movement(generator);
        users.push_back(std::make_unique<User>(cash));
    }
}

const std::vector<std::unique_ptr<Stock>>& MarketState::getStocks() const {
    return this->stocks;
}
const std::vector<std::unique_ptr<User>>& MarketState::getUsers() const {
    return this->users;
}

int MarketState::setStockPrice(const std::string& symbol, double price, double currentTime) {
    for (auto& stock : this->stocks) {
        if (stock->getSymbol() == symbol) {
            stock->setPrice(price, currentTime);
            return 0;
        }
    }
    return -1;
}
