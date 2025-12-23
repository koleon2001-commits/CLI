#pragma once
#include <optional>
#include <string>
#include "log_analyzer/record.h"

namespace loga {

// Parses a single log line. Returns nullopt if the line is blank/comment/invalid.
// Expected: key=value fields separated by spaces. Requires latency_ms=<number>.
// Error if status=ERROR (case-insensitive) OR error=true (case-insensitive).
std::optional<Record> parse_line(const std::string& line);

} // namespace loga
