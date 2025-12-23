#include "log_analyzer/parser.h"
#include "log_analyzer/stats.h"

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
void usage(const char* argv0) {
  std::cerr
    << "Usage: " << argv0 << " --input <path> [--demo-crash]\n"
    << "  --input <path>     Path to log file\n"
    << "  --demo-crash       Intentionally crash (for gdb demo)\n";
}
} // namespace

int main(int argc, char** argv) {
  std::string input;
  bool demo_crash = false;

  for (int i = 1; i < argc; ++i) {
    std::string a = argv[i];
    if (a == "--input" && i + 1 < argc) {
      input = argv[++i];
    } else if (a == "--demo-crash") {
      demo_crash = true;
    } else if (a == "--help" || a == "-h") {
      usage(argv[0]);
      return 0;
    } else {
      std::cerr << "Unknown arg: " << a << "\n";
      usage(argv[0]);
      return 2;
    }
  }

  if (demo_crash) {
    int* p = nullptr; // intentional crash
    *p = 42;
  }

  if (input.empty()) {
    std::cerr << "Missing --input\n";
    usage(argv[0]);
    return 2;
  }

  std::ifstream in(input);
  if (!in) {
    std::cerr << "Failed to open: " << input << "\n";
    return 1;
  }

  std::vector<double> latencies;
  std::size_t errors = 0;

  std::string line;
  while (std::getline(in, line)) {
    auto rec = loga::parse_line(line);
    if (!rec) continue;
    latencies.push_back(rec->latency_ms);
    if (rec->is_error) errors++;
  }

  auto s = loga::summarize(latencies, errors);

  if (s.count == 0) {
    std::cout << "count=0 errors=0 error_rate=0%\n";
    return 0;
  }

  const double err_rate = (static_cast<double>(s.errors) / static_cast<double>(s.count)) * 100.0;

  std::cout.setf(std::ios::fixed);
  std::cout.precision(2);

  std::cout << "count=" << s.count
            << " errors=" << s.errors
            << " error_rate=" << err_rate << "%\n";
  std::cout << "latency_ms: min=" << s.min_ms
            << " mean=" << s.mean_ms
            << " max=" << s.max_ms << "\n";
  std::cout << "p50=" << s.p50_ms
            << " p95=" << s.p95_ms
            << " p99=" << s.p99_ms << "\n";

  return 0;
}
