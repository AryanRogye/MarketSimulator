#ifndef MARKETSIM_H
#define MARKETSIM_H

#include "MarketState.h"
#include <memory>
#include <thread>
class MarketSim {
public:
    MarketSim(std::shared_ptr<MarketState> marketState);
    void beginSimulation();
private:
    std::shared_ptr<MarketState> marketState;
    std::thread simulationThread;
};

#endif // MARKETSIM_H
