#include "MarketSim.h"
#include <chrono>
#include <random>
#include <iostream>

MarketSim::MarketSim(std::shared_ptr<MarketState> marketState)
    : marketState(marketState)
{
    
}

// void MarketSim::beginSimulation()
// {
//     simulationThread = std::thread([this]() {
        
//         auto startTime = std::chrono::steady_clock::now();
//         auto lastUpdateTime = startTime;
//         std::mt19937 generator(std::random_device{}());
//         std::uniform_real_distribution<double> movement(-1, 1);

//         /// this is just a test not what the final end simulation will look like
//         /// we'll have another thread simulate users who are buying and selling stocks
//         /// which will update the market state accordingly
//         while (true) {
//             auto now = std::chrono::steady_clock::now();

//             if (now - lastUpdateTime >= std::chrono::milliseconds(100)) {
//                 for (auto& stockInfo: marketState->getStocks()) {

//                     Stock& stock = *stockInfo.first;
                    
//                     double newPrice = stock.getPrice() + movement(generator);
//                     double currentTime = stock.getCurrentTime() + 0.1;

//                     if (marketState->setStockPrice(stock.getSymbol(), newPrice, currentTime) != 0) {
//                         std::cerr << "Failed to set stock price for " << stock.getSymbol() << std::endl;
//                     }

//                 }
//                 lastUpdateTime = now;
//             }
//         }
//     });
// }

void MarketSim::beginSimulation()
{
    simulationThread = std::thread([this]() {
        
        auto startTime = std::chrono::steady_clock::now();
        auto lastUpdateTime = startTime;
        std::mt19937 generator(std::random_device{}());
        std::uniform_real_distribution<double> movement(-1, 1);

        /// 0 == buy, 1 == sell, >= 2 == hold
        std::uniform_int_distribution<int> buyOrSell(0, 100);

        /// this is just a test not what the final end simulation will look like
        /// we'll have another thread simulate users who are buying and selling stocks
        /// which will update the market state accordingly
        while (true) {
            auto now = std::chrono::steady_clock::now();

            if (now - lastUpdateTime >= std::chrono::milliseconds(100)) {
                for (auto& user: marketState->getUsers()) {
                    
                    const std::unordered_map<std::string, std::vector<Order>> orders = user->get_orders();
                    /// the user now has their orders, so we can randomly choose to either buy a stock or sell a stock
                    int tradeDecision = buyOrSell(generator);
                    double currentTime = std::chrono::duration<double>(now - startTime).count();
                    if (tradeDecision == 0) {
                        /// buy
                        marketState->buyRandomOrder(*user, currentTime);
                    }
                    if (tradeDecision == 1) {
                        /// sell
                        marketState->sellRandomOrder(*user, currentTime);
                    }
                    if (tradeDecision >= 2) {
                        /// hold
                        continue;
                    }

                    // Stock& stock = *stockInfo.first;
                    
                    // double newPrice = stock.getPrice() + movement(generator);
                    // double currentTime = stock.getCurrentTime() + 0.1;

                    // if (marketState->setStockPrice(stock.getSymbol(), newPrice, currentTime) != 0) {
                    //     std::cerr << "Failed to set stock price for " << stock.getSymbol() << std::endl;
                    // }

                }
                lastUpdateTime = now;
            }
        }
    });
}
