#pragma once

#include <string>
#include <vector>

namespace sattyre {

struct SMTProblem {
  std::string smtlib;
};

enum class SMTSolveResult {
  Sat,
  Unsat,
  Unknown
};

struct SMTModel {
  std::vector<std::string> bindings;
};

class SMTSolver {
public:
  virtual ~SMTSolver() = default;
  virtual void load(const SMTProblem& problem) = 0;
  virtual SMTSolveResult solve() = 0;
  virtual SMTModel model() const = 0;
};

SMTProblem parse_smtlib_file(const std::string& path);

} // namespace sattyre
