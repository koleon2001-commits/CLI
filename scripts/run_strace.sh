#!/usr/bin/env bash
set -euo pipefail
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
strace -f -o strace.log ./build/log_analyzer --input data/sample.log >/dev/null
echo "Saved syscall trace to strace.log"
