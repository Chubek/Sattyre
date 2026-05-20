#include "sattyre/Sattyre-Solver.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  if (sattyre_solver_abi_compatible(SATTYRE_SOLVER_ABI_VERSION) != 1) {
    throw std::runtime_error("expected ABI compatibility for current version");
  }

  if (sattyre_solver_abi_compatible(SATTYRE_SOLVER_ABI_VERSION + 1u) != 0) {
    throw std::runtime_error("expected ABI incompatibility for newer version");
  }

  const SattyreSolverInfo info = sattyre_solver_info();
  if (sattyre_solver_abi_compatible(info.abi_version) != 1) {
    throw std::runtime_error("expected ABI compatibility for solver info version");
  }

  std::cout << "Solver ABI compatibility tests passed\n";
  return EXIT_SUCCESS;
}
