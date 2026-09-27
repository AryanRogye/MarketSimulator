#include <MarketSim.h>
#include <MarketState.h>
#include <ui.h>

int main() {
    std::shared_ptr<MarketState> marketState = std::make_shared<MarketState>();

    UI ui(marketState);
    MarketSim sim(marketState);

    sim.beginSimulation();
    ui.begin();

    return 0;
}
