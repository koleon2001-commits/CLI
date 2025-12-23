#include "log_analyzer/stats.h"

#include <cmath>
#include <iostream>
#include <vector>

static bool nearly(double a, double b, double eps = 1e-9) {
  return std::fabs(a - b) <= eps;
}

int main() {
  {
    std::vector<double> v{10, 20, 30, 40};
    double p50 = loga::percentile_inc(v, 0.5);
    if (!nearly(p50, 25.0)) {
      std::cerr << "p50 expected 25 got " << p50 << "\n";
      return 1;
    }
  }
  {
    std::vector<double> v{1};
    if (!nearly(loga::percentile_inc(v, 0.99), 1.0)) return 1;
  }
  {
    std::vector<double> v{8.5, 12.3, 85.1};
    auto s = loga::summarize(v, 1);
    if (s.count != 3 || s.errors != 1) return 1;
    if (!nearly(s.min_ms, 8.5)) return 1;
    if (!nearly(s.max_ms, 85.1)) return 1;
  }

  std::cout << "test_stats OK\n";
  return 0;
}
