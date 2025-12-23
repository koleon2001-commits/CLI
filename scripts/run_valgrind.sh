#!/usr/bin/env bash
set -euo pipefail
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
valgrind --leak-check=full --show-leak-kinds=all --error-exitcode=1 ./build/log_analyzer --input data/sample.log
echo "valgrind OK"
