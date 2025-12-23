#pragma once
#include <string>

namespace loga {

struct Record {
  double latency_ms = 0.0;
  bool is_error = false;
  std::string raw_line;
};

} // namespace loga
