#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace sattyre {

using Literal = std::int32_t;
using Clause = std::vector<Literal>;

struct SATProblem {
  std::vector<Clause> clauses;
  std::size_t variable_count = 0;
};

enum class SolveResult {
  Sat,
  Unsat,
  Unknown
};

struct SATModel {
  std::vector<int> assignments;
};

class SATSolver {
public:
  virtual ~SATSolver() = default;
  virtual void load(const SATProblem& problem) = 0;
  virtual SolveResult solve() = 0;
  virtual SATModel model() const = 0;
};

SATProblem parse_dimacs_file(const std::string& path);

} // namespace sattyre
