#include <cstdlib>
#include <stdexcept>
#include <string>

int run_command(const std::string& command) {
  return std::system(command.c_str());
}

int main() {
  const int no_args = run_command("./sattyre-registry > /tmp/sattyre-registry-noargs.out 2>/tmp/sattyre-registry-noargs.err");
  if (no_args == 0) {
    throw std::runtime_error("expected non-zero exit for no args");
  }

  const int bad_cmd = run_command("./sattyre-registry nope > /tmp/sattyre-registry-badcmd.out 2>/tmp/sattyre-registry-badcmd.err");
  if (bad_cmd == 0) {
    throw std::runtime_error("expected non-zero exit for bad command");
  }

  return EXIT_SUCCESS;
}
