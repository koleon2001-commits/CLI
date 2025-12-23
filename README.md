![ci](https://github.com/koleon2001-commits/CLI/actions/workflows/ci.yml/badge.svg)

# CLI Log Analyzer (C++17) — Systems Tooling Demo for Data Center Simulation

This repository is a **systems-oriented CLI** that parses request logs and reports reliability + latency KPIs
(**error rate, min/mean/max, p50/p95/p99**). It is intentionally structured to mirror the day-to-day work of
a **Data Center Simulation / Platform Engineering** internship: **reproducible builds, test automation, debugging,
and performance-minded implementation**.

## Why this project (alignment to NVIDIA Data Center Simulation Intern)

This project demonstrates practical skills that map directly to the role’s responsibilities:

- **Verify / Debug / Fix**: robust parsing with unit tests; debugging workflows via `gdb` and `valgrind`
- **Automation**: one-command build/test; scripts for common investigation tasks
- **CI/CD**: GitHub Actions pipeline runs build + tests + valgrind smoke check + Docker image build
- **Containers**: multi-stage Docker build for consistent, portable execution
- **Linux / System thinking**: tooling (`strace`) and careful error handling (fail-soft parsing, clear exit codes)

## Features

- Parses `key=value` fields per line (ignores blank lines and `#` comments)
- Tracks:
  - total request count
  - error count / error rate
  - latency distribution (min/mean/max, p50/p95/p99)
- Includes:
  - **CMake** build
  - **CTest** unit tests (parser + percentile implementation)
  - **GitHub Actions** CI (build/test/valgrind/docker-build)
  - **Dockerfile** (multi-stage) for reproducible runs
  - OS tooling scripts: `valgrind`, `gdb` demo, `strace`

## Log format

One line per request (blank lines and `#` comments ignored). Key-value fields separated by spaces:

```
ts=2026-01-01T00:00:00Z method=GET path=/v1/ping status=OK latency_ms=12.3
ts=2026-01-01T00:00:01Z method=GET path=/v1/ping status=ERROR latency_ms=85.1
```

Required: `latency_ms=<number>`  
Error if: `status=ERROR` (case-insensitive) OR `error=true`

Sample log: `data/sample.log`

## Build & Test (local)

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
ctest --test-dir build --output-on-failure
```

## Run

```bash
./build/log_analyzer --input data/sample.log
```

Example output:

```
count=12 errors=2 error_rate=16.67%
latency_ms: min=8.50 mean=27.41 max=85.10
p50=16.90 p95=78.34 p99=84.27
```

## CI/CD (GitHub Actions)

Workflow: `.github/workflows/ci.yml`

Runs on every push / pull request:

1. Configure + build (Release)
2. Unit tests (CTest)
3. **Valgrind smoke check** (Debug build) to catch leaks/invalid memory access
4. Docker image build (portable execution)

## Docker (reproducible execution)

```bash
docker build -t cli-log-analyzer .
docker run --rm cli-log-analyzer --input /app/data/sample.log
```

## Debugging & OS Tooling

### Valgrind (memory correctness)
```bash
./scripts/run_valgrind.sh
```

### GDB demo (intentional crash for backtrace practice)
```bash
./scripts/demo_gdb.sh
# In gdb: run --demo-crash
```

### Strace demo (syscall trace)
```bash
./scripts/run_strace.sh
# Outputs: strace.log
```

## Implementation notes

- Percentiles use sorted values with **linear interpolation** (similar to Excel `PERCENTILE.INC`).
- Current approach loads all latencies into memory and sorts (**O(n log n)**). For very large logs, this can be
  extended to streaming/approximate quantiles (e.g., t-digest / CKMS) or external sorting.

## Resume bullets (copy/paste)

- Built a **C++17 log analysis CLI** to compute reliability and latency KPIs (error rate, p50/p95/p99) for datacenter-style request logs.
- Implemented **unit-tested parsing + percentile statistics**, and automated build/test via **GitHub Actions CI** with **valgrind** memory checks.
- Packaged the tool with a **multi-stage Docker build** and added Linux investigation scripts (`gdb`, `strace`) for reproducible debugging workflows.
