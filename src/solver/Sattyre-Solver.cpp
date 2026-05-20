#include "sattyre/Sattyre-Solver.hpp"
#include "sattyre/Sattyre-Solver.h"
#include "sattyre/Log.hpp"
#include <cstdlib>
#include <cstdint>
#include <stdexcept>

namespace sattyre {

struct NativeSolverAdapter::Impl {
  SATProblem problem;
  SATModel last_model;
  SolveResult last_result = SolveResult::Unknown;
};

namespace {

bool clause_satisfied(const Clause& clause, std::uint64_t mask) {
  for (const Literal literal : clause) {
    const std::size_t variable = static_cast<std::size_t>(std::abs(literal));
    if (variable == 0) {
      continue;
    }
    const std::uint64_t bit = std::uint64_t{1} << (variable - 1);
    const bool value = (mask & bit) != 0;
    if ((literal > 0 && value) || (literal < 0 && !value)) {
      return true;
    }
  }
  return false;
}

bool formula_satisfied(const SATProblem& problem, std::uint64_t mask) {
  for (const Clause& clause : problem.clauses) {
    if (!clause_satisfied(clause, mask)) {
      return false;
    }
  }
  return true;
}

} // namespace

NativeSolverAdapter::NativeSolverAdapter()
  : impl_(new Impl{}) {
  log::info("NativeSolverAdapter created");
}

NativeSolverAdapter::~NativeSolverAdapter() = default;

void NativeSolverAdapter::load(const SATProblem& problem) {
  impl_->problem = problem;
}

SolveResult NativeSolverAdapter::solve() {
  const SattyreSolverInfo info = sattyre_solver_info();
  if (!sattyre_solver_abi_compatible(info.abi_version)) {
    throw std::runtime_error("Native solver ABI version is incompatible");
  }

  if (impl_->problem.variable_count > 63) {
    log::warn("NativeSolverAdapter supports up to 63 variables in this baseline implementation");
    impl_->last_model = {};
    impl_->last_result = SolveResult::Unknown;
    return impl_->last_result;
  }

  const std::uint64_t limit = std::uint64_t{1} << impl_->problem.variable_count;
  for (std::uint64_t mask = 0; mask < limit; ++mask) {
    if (!formula_satisfied(impl_->problem, mask)) {
      continue;
    }

    impl_->last_model.assignments.assign(impl_->problem.variable_count + 1, 0);
    for (std::size_t variable = 1; variable <= impl_->problem.variable_count; ++variable) {
      const std::uint64_t bit = std::uint64_t{1} << (variable - 1);
      impl_->last_model.assignments[variable] = (mask & bit) != 0 ? 1 : -1;
    }
    impl_->last_result = SolveResult::Sat;
    return impl_->last_result;
  }

  impl_->last_model = {};
  impl_->last_result = SolveResult::Unsat;
  return impl_->last_result;
}

SATModel NativeSolverAdapter::model() const {
  return impl_->last_model;
}

} // namespace sattyre
