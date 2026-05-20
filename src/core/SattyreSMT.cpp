#include "sattyre/SattyreSMT.hpp"
#include "sattyre/Log.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>

namespace sattyre {

SMTProblem parse_smtlib_file(const std::string& path) {
  log::info("Parsing SMT-LIB file: " + path);

  SMTProblem p;
  std::ifstream in(path);
  if (!in) {
throw std::runtime_error("failed to open SMT-LIB file: " + path);
  }

  std::ostringstream ss;
  ss << in.rdbuf();
  p.smtlib = ss.str();
  return p;
}

} // namespace sattyre
