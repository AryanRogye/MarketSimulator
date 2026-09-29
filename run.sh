#!/bin/bash
set -e
set -o pipefail

# cmake --build build 2>&1 | grep -B 5 -A 10 "error:"
cmake --build build
./build/MarketSim
