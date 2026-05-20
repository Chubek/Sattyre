#include "sattyre/Sattyre-Solver.h"
#include <stdlib.h>

struct SattyreSolver {
  int reserved;
};

SattyreSolver* sattyre_solver_create(void) {
  return (SattyreSolver*)calloc(1, sizeof(SattyreSolver));
}

void sattyre_solver_destroy(SattyreSolver* solver) {
  free(solver);
}

SattyreSolverResult sattyre_solver_solve(SattyreSolver* solver) {
  (void)solver;
  return SATTYRE_SOLVER_UNKNOWN;
}

SattyreSolverInfo sattyre_solver_info(void) {
  SattyreSolverInfo info;
  info.abi_version = SATTYRE_SOLVER_ABI_VERSION;
  info.name = "SattyreNativeSolver";
  info.version = "0.1.0";
  return info;
}

int sattyre_solver_abi_compatible(unsigned int abi_version) {
  return abi_version == SATTYRE_SOLVER_ABI_VERSION;
}
