#include <MarketState.h>

MarketState::MarketState() {
    this->stocks.push_back(std::make_unique<Stock>("AAPL", 180.00));
    this->stocks.push_back(std::make_unique<Stock>("MSFT", 420.00));
    this->stocks.push_back(std::make_unique<Stock>("NVDA", 150.00));
    
}

const std::vector<std::unique_ptr<Stock>>& MarketState::getStocks() const {
    return this->stocks;
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
