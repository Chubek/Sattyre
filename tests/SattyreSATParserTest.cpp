#include "sattyre/SattyreSAT.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using sattyre::parse_dimacs_file;

static std::filesystem::path write_fixture(const std::string& contents, const char* name) {
  const auto path = std::filesystem::temp_directory_path() / name;
  std::ofstream out(path);
  out << contents;
  return path;
}

static void expect_throw(const std::string& contents, const char* name) {
  const auto path = write_fixture(contents, name);
  bool threw = false;
  try {
    (void)parse_dimacs_file(path.string());
  } catch (const std::runtime_error&) {
    threw = true;
  }
  if (!threw) {
    throw std::runtime_error(std::string("expected failure for ") + name);
  }
}

int main() {
  {
    const auto path = write_fixture("c ok\np cnf 3 2\n1 -2 0\n2 3 0\n", "sattyre-ok.cnf");
    const auto problem = parse_dimacs_file(path.string());
    if (problem.variable_count != 3 || problem.clauses.size() != 2) {
      throw std::runtime_error("unexpected parse result");
    }
  }
  {
    const auto path = write_fixture("\n c with leading space\n\np cnf 2 1\n\n 1 -2 0\n", "sattyre-comments-blank.cnf");
    const auto problem = parse_dimacs_file(path.string());
    if (problem.variable_count != 2 || problem.clauses.size() != 1 || problem.clauses[0].size() != 2) {
      throw std::runtime_error("unexpected parse result with comments and blanks");
    }
  }

  expect_throw("p cnf 1 1\n1\n", "sattyre-missing-zero.cnf");
  expect_throw("p cnf 1 1\n2 0\n", "sattyre-out-of-range.cnf");
  expect_throw("p cnf 1 2\n1 0\n", "sattyre-bad-count.cnf");
  expect_throw("1 0\np cnf 1 1\n1 0\n", "sattyre-clause-before-header.cnf");

  std::cout << "SAT parser tests passed\n";
  return EXIT_SUCCESS;
}
