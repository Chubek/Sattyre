#pragma once

#include <memory>
#include "sattyre/SattyreSAT.hpp"

namespace sattyre {

class NativeSolverAdapter : public SATSolver {
public:
  NativeSolverAdapter();
  ~NativeSolverAdapter() override;

  void load(const SATProblem& problem) override;
  SolveResult solve() override;
  SATModel model() const override;

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};

} // namespace sattyre
