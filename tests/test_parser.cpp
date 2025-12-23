#include "log_analyzer/parser.h"

#include <iostream>

int main() {
  {
    auto r = loga::parse_line("ts=2026-01-01Z status=OK latency_ms=12.3");
    if (!r) return 1;
    if (r->is_error) return 1;
    if (r->latency_ms != 12.3) return 1;
  }
  {
    auto r = loga::parse_line("status=ERROR latency_ms=85.1");
    if (!r) return 1;
    if (!r->is_error) return 1;
  }
  {
    auto r = loga::parse_line("# comment");
    if (r) return 1;
  }
  {
    auto r = loga::parse_line("status=OK");
    if (r) return 1;
  }

  std::cout << "test_parser OK\n";
  return 0;
}
