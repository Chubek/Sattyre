#ifndef SATTYRE_SOLVER_H
#define SATTYRE_SOLVER_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SattyreSolver SattyreSolver;

typedef enum {
  SATTYRE_SOLVER_SAT = 0,
  SATTYRE_SOLVER_UNSAT = 1,
  SATTYRE_SOLVER_UNKNOWN = 2
} SattyreSolverResult;

#define SATTYRE_SOLVER_ABI_VERSION 1u

typedef struct {
  unsigned int abi_version;
  const char* name;
  const char* version;
} SattyreSolverInfo;

SattyreSolver* sattyre_solver_create(void);
void sattyre_solver_destroy(SattyreSolver* solver);
SattyreSolverResult sattyre_solver_solve(SattyreSolver* solver);
SattyreSolverInfo sattyre_solver_info(void);
int sattyre_solver_abi_compatible(unsigned int abi_version);

#ifdef __cplusplus
}
#endif

#endif
