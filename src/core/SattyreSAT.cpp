#include "sattyre/SattyreSAT.hpp"
#include "sattyre/Log.hpp"

#include <fstream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>

namespace sattyre {

SATProblem parse_dimacs_file(const std::string& path) {
  log::info("Parsing DIMACS file: " + path);

  SATProblem p;
  std::ifstream in(path);
  if (!in) {
throw std::runtime_error("failed to open DIMACS file: " + path);
  }

  bool header_seen = false;
  std::size_t expected_clause_count = 0;
  std::size_t actual_clause_count = 0;

  auto invalid = [&](const std::string& message) -> std::runtime_error {
    return std::runtime_error("invalid DIMACS in " + path + ": " + message);
  };

  std::string line;
  std::size_t line_no = 0;
  while (std::getline(in, line)) {
    ++line_no;
    std::istringstream line_stream(line);
    std::string first_token;
    if (!(line_stream >> first_token)) {
      continue;
    }
    if (first_token == "c") {
      continue;
    }
    if (first_token == "p") {
      if (header_seen) {
        throw invalid("duplicate problem line at line " + std::to_string(line_no));
      }
      std::string format;
      std::size_t variable_count = 0;
      std::size_t clause_count = 0;
      if (!(line_stream >> format >> variable_count >> clause_count) || format != "cnf") {
        throw invalid("malformed problem line at line " + std::to_string(line_no));
      }
      std::string trailing;
      if (line_stream >> trailing) {
        throw invalid("unexpected token in problem line at line " + std::to_string(line_no));
      }
      p.variable_count = variable_count;
      expected_clause_count = clause_count;
      header_seen = true;
      continue;
    }

    if (!header_seen) {
      throw invalid("clause appears before problem line at line " + std::to_string(line_no));
    }

    line_stream.clear();
    line_stream.str(line);

    Clause clause;
    bool terminated = false;
    Literal lit = 0;
    while (line_stream >> lit) {
      if (lit == 0) {
        terminated = true;
        break;
      }
      if (lit == std::numeric_limits<Literal>::min()) {
        throw invalid("literal out of range at line " + std::to_string(line_no));
      }
      const auto abs_lit = static_cast<std::size_t>(lit < 0 ? -lit : lit);
      if (abs_lit == 0 || abs_lit > p.variable_count) {
        throw invalid("literal exceeds declared variable count at line " + std::to_string(line_no));
      }
      clause.push_back(lit);
    }
    if (!terminated) {
      throw invalid("clause missing terminating 0 at line " + std::to_string(line_no));
    }
    if (!line_stream.eof()) {
      throw invalid("non-integer token in clause at line " + std::to_string(line_no));
    }

    p.clauses.push_back(std::move(clause));
    ++actual_clause_count;
    if (actual_clause_count > expected_clause_count) {
      throw invalid("more clauses than declared in problem line");
    }
  }

  if (!header_seen) {
    throw invalid("missing problem line");
  }
  if (actual_clause_count != expected_clause_count) {
    throw invalid("clause count does not match problem line");
  }

  return p;
}

} // namespace sattyre
