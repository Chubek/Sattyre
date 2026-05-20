#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>

namespace {

void write_text(const std::filesystem::path& path, const std::string& contents) {
  std::filesystem::create_directories(path.parent_path());
  std::ofstream out(path, std::ios::binary);
  if (!out) {
    throw std::runtime_error("failed to write " + path.string());
  }
  out << contents;
}

int run_command(const std::string& command) {
  return std::system(command.c_str());
}

} // namespace

int main() {
  const std::filesystem::path temp_root = std::filesystem::temp_directory_path() / "sattyre-cli-smoke";
  const std::filesystem::path cnf_path = temp_root / "demo.cnf";

  std::error_code ignore;
  std::filesystem::remove_all(temp_root, ignore);
  std::filesystem::create_directories(temp_root);

  write_text(cnf_path,
             "c demo\n"
             "p cnf 1 1\n"
             "1 0\n");

  const int status = run_command("./sattyre-cli " + cnf_path.string());
  if (status != 0) {
    throw std::runtime_error("sattyre-cli failed");
  }

  return EXIT_SUCCESS;
}
