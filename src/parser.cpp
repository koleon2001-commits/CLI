#include "log_analyzer/parser.h"

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <unordered_map>

namespace loga {
namespace {

static inline std::string trim(const std::string& s) {
  std::size_t start = 0;
  while (start < s.size() && std::isspace(static_cast<unsigned char>(s[start]))) start++;
  std::size_t end = s.size();
  while (end > start && std::isspace(static_cast<unsigned char>(s[end - 1]))) end--;
  return s.substr(start, end - start);
}

static inline std::string to_lower(std::string s) {
  std::transform(s.begin(), s.end(), s.begin(),
                 [](unsigned char c){ return static_cast<char>(std::tolower(c)); });
  return s;
}

} // namespace

std::optional<Record> parse_line(const std::string& line) {
  std::string t = trim(line);
  if (t.empty()) return std::nullopt;
  if (t[0] == '#') return std::nullopt;

  std::unordered_map<std::string, std::string> kv;
  std::istringstream iss(t);
  std::string token;
  while (iss >> token) {
    auto pos = token.find('=');
    if (pos == std::string::npos) continue;
    auto key = token.substr(0, pos);
    auto val = token.substr(pos + 1);
    if (!key.empty()) kv[to_lower(key)] = val;
  }

  auto it = kv.find("latency_ms");
  if (it == kv.end()) return std::nullopt;

  double latency = 0.0;
  try {
    latency = std::stod(it->second);
  } catch (...) {
    return std::nullopt;
  }

  bool is_error = false;
  auto st = kv.find("status");
  if (st != kv.end() && to_lower(st->second) == "error") is_error = true;

  auto er = kv.find("error");
  if (er != kv.end()) {
    auto v = to_lower(er->second);
    if (v == "true" || v == "1" || v == "yes") is_error = true;
  }

  Record r;
  r.latency_ms = latency;
  r.is_error = is_error;
  r.raw_line = t;
  return r;
}

} // namespace loga
