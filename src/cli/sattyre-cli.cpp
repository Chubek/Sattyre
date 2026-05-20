#include "sattyre/Log.hpp"
#include "sattyre/Sattyre-Solver.hpp"
#include "sattyre/SattyreSAT.hpp"
#include "sattyre/SattyreSMT.hpp"
#include "sattyre/Version.hpp"

#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

namespace {

const char* solve_result_text(sattyre::SolveResult result) {
  switch (result) {
    case sattyre::SolveResult::Sat: return "sat";
    case sattyre::SolveResult::Unsat: return "unsat";
    case sattyre::SolveResult::Unknown: return "unknown";
  }
  return "unknown";
}

} // namespace

int main(int argc, char** argv) {
  sattyre::log::init();
  sattyre::log::info("sattyre-cli starting");

  if (argc < 2) {
    std::cout << "sattyre-cli " << SATTYRE_VERSION_STRING << "\n";
    std::cout << "usage: sattyre-cli <problem-file>\n";
    return 1;
  }

  const std::filesystem::path input_path(argv[1]);
  try {
    if (input_path.extension() == ".smt2" || input_path.extension() == ".smt") {
      const auto problem = sattyre::parse_smtlib_file(input_path.string());
      std::cout << "sattyre-cli " << SATTYRE_VERSION_STRING << "\n";
      std::cout << "smt-bytes: " << problem.smtlib.size() << "\n";
      return 0;
    }

    const auto problem = sattyre::parse_dimacs_file(input_path.string());
    sattyre::NativeSolverAdapter solver;
    solver.load(problem);
    const auto result = solver.solve();
    std::cout << "sattyre-cli " << SATTYRE_VERSION_STRING << "\n";
    std::cout << "result: " << solve_result_text(result) << "\n";
    std::cout << "sat-clauses: " << problem.clauses.size() << "\n";
    std::cout << "sat-variables: " << problem.variable_count << "\n";
    return 0;
  } catch (const std::exception& error) {
    std::cerr << "sattyre-cli: " << error.what() << "\n";
    return 2;
  }
}
