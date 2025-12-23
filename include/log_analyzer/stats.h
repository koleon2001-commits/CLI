#pragma once
#include <cstddef>
#include <vector>

namespace loga {

struct Summary {
  std::size_t count = 0;
  std::size_t errors = 0;
  double min_ms = 0.0;
  double max_ms = 0.0;
  double mean_ms = 0.0;
  double p50_ms = 0.0;
  double p95_ms = 0.0;
  double p99_ms = 0.0;
};

double percentile_inc(std::vector<double> values, double p); // p in [0,1]
Summary summarize(const std::vector<double>& latencies, std::size_t errors);

} // namespace loga
