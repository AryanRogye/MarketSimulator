# MarketSim

MarketSim is a small C++ practice project that simulates stock price changes and displays them in a desktop UI. It starts with three sample stocks (AAPL, MSFT, and NVDA), updates their prices with random movements, and plots their price history.

This is a work in progress for learning C++, threads, and immediate mode UI. The prices are fictional and the simulation is not a market model.

## Build and run

You'll need a C++17 compiler, CMake 3.21 or newer, Git, and OpenGL development support. CMake downloads GLFW, Dear ImGui, and ImPlot during the first configure, so that step needs an internet connection.

```sh
cmake -S . -B build
cmake --build build
./build/MarketSim
```

After configuring once, `./run.sh` rebuilds and launches the app. On a system with LLDB, `./run_debug.sh` configures a debug build and opens it in the debugger.

## Project layout

- `src/MarketSim.cpp` runs the price update loop.
- `src/MarketState.cpp` holds the sample stocks.
- `src/Stock.cpp` stores each stock's price history.
- `src/ui.cpp` draws the stock list and plots with Dear ImGui and ImPlot.

The Users panel is a placeholder for future experiments.
