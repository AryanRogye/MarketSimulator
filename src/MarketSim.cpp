#include "MarketSim.h"
#include <chrono>
#include <random>
#include <iostream>

MarketSim::MarketSim(std::shared_ptr<MarketState> marketState)
    : marketState(marketState)
{
    
}

void MarketSim::beginSimulation()
{
    simulationThread = std::thread([this]() {
        
        auto startTime = std::chrono::steady_clock::now();
        auto lastUpdateTime = startTime;
        std::mt19937 generator(std::random_device{}());
        std::uniform_real_distribution<double> movement(-1, 1);

        /// this is just a test not what the final end simulation will look like
        /// we'll have another thread simulate users who are buying and selling stocks
        /// which will update the market state accordingly
        while (true) {
            auto now = std::chrono::steady_clock::now();

            if (now - lastUpdateTime >= std::chrono::milliseconds(100)) {
                for (auto& stock: marketState->getStocks()) {
                    double newPrice = stock->getPrice() + movement(generator);
                    double currentTime = stock->getCurrentTime() + 0.1;

                    if (marketState->setStockPrice(stock->getSymbol(), newPrice, currentTime) != 0) {
                        std::cerr << "Failed to set stock price for " << stock->getSymbol() << std::endl;
                    }

                }
                lastUpdateTime = now;
            }
        }
    });
}
