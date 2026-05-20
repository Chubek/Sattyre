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
  namespace fs = std::filesystem;
  const fs::path temp_root = fs::temp_directory_path() / "sattyre-packman-cli";
  const fs::path source_dir = temp_root / "src";
  const fs::path package_dir = temp_root / "package";
  const fs::path bundle_path = temp_root / "bundle.tar.gz";
  const fs::path install_root = temp_root / "install";

  std::error_code ignore;
  fs::remove_all(temp_root, ignore);
  fs::create_directories(source_dir);

  write_text(source_dir / "manifest.json",
             "{\n"
             "  \"name\": \"cli-solver\",\n"
             "  \"version\": \"1.0.0\",\n"
             "  \"type\": \"solver\"\n"
             "}\n");
  write_text(source_dir / "bin" / "solver.txt", "payload\n");

  if (run_command("./sattyre-packman validate " + source_dir.string()) != 0) {
    throw std::runtime_error("validate failed");
  }
  if (run_command("./sattyre-packman pack " + source_dir.string() + " " + bundle_path.string()) != 0) {
    throw std::runtime_error("pack failed");
  }
  if (run_command("./sattyre-packman unpack " + bundle_path.string() + " " + package_dir.string()) != 0) {
    throw std::runtime_error("unpack failed");
  }
  if (!fs::exists(package_dir / "manifest.json")) {
    throw std::runtime_error("unpack did not create manifest");
  }
  if (run_command("./sattyre-packman install " + bundle_path.string() + " --root " + install_root.string()) != 0) {
    throw std::runtime_error("install failed");
  }
  if (!fs::exists(install_root / "lib" / "solvers" / "cli-solver" / "manifest.json")) {
    throw std::runtime_error("install did not create solver manifest");
  }
  if (run_command("./sattyre-packman remove cli-solver --root " + install_root.string()) != 0) {
    throw std::runtime_error("remove failed");
  }
  if (fs::exists(install_root / "lib" / "solvers" / "cli-solver")) {
    throw std::runtime_error("remove left package behind");
  }

  return EXIT_SUCCESS;
}
