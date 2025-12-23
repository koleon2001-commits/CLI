#include "log_analyzer/stats.h"

#include <algorithm>
#include <cmath>
#include <numeric>
#include <stdexcept>

namespace loga {

double percentile_inc(std::vector<double> values, double p) {
  if (values.empty()) throw std::invalid_argument("percentile on empty set");
  if (p < 0.0 || p > 1.0) throw std::invalid_argument("p must be in [0,1]");

  std::sort(values.begin(), values.end());
  const std::size_t n = values.size();

  if (n == 1) return values[0];
  if (p == 0.0) return values.front();
  if (p == 1.0) return values.back();

  const double idx = (static_cast<double>(n) - 1.0) * p;
  const std::size_t lo = static_cast<std::size_t>(std::floor(idx));
  const std::size_t hi = static_cast<std::size_t>(std::ceil(idx));
  const double frac = idx - static_cast<double>(lo);

  if (hi >= n) return values.back();
  return values[lo] + (values[hi] - values[lo]) * frac;
}

Summary summarize(const std::vector<double>& latencies, std::size_t errors) {
  Summary s;
  s.count = latencies.size();
  s.errors = errors;

  if (latencies.empty()) return s;

  auto [mn_it, mx_it] = std::minmax_element(latencies.begin(), latencies.end());
  s.min_ms = *mn_it;
  s.max_ms = *mx_it;
  s.mean_ms = std::accumulate(latencies.begin(), latencies.end(), 0.0) / static_cast<double>(latencies.size());

  s.p50_ms = percentile_inc(latencies, 0.50);
  s.p95_ms = percentile_inc(latencies, 0.95);
  s.p99_ms = percentile_inc(latencies, 0.99);
  return s;
}

} // namespace loga
