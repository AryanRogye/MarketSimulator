#!/bin/bash
set -e

cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
lldb ./build/MarketSim
