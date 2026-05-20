#include "sattyre/Bundle.hpp"
#include "sattyre/Installer.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
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

} // namespace

int main() {
  namespace fs = std::filesystem;
  const fs::path temp_root = fs::temp_directory_path() / "sattyre-package-flow";
  const fs::path src_dir = temp_root / "src";
  const fs::path bundle_path = temp_root / "bundle.tar.gz";
  const fs::path install_root = temp_root / "install-root";

  std::error_code ignore;
  fs::remove_all(temp_root, ignore);
  fs::create_directories(src_dir);

  write_text(src_dir / "manifest.json",
             "{\n"
             "  \"name\": \"mini-solver\",\n"
             "  \"version\": \"0.1.0\",\n"
             "  \"type\": \"solver\",\n"
             "  \"dependencies\": [\"fmt\"]\n"
             "}\n");
  write_text(src_dir / "bin" / "solver.txt", "solver payload\n");

  if (!sattyre::create_bundle(src_dir.string(), bundle_path.string())) {
    throw std::runtime_error("create_bundle failed");
  }
  if (sattyre::install_package(bundle_path.string(), install_root.string()) != sattyre::InstallerStatus::Ok) {
    throw std::runtime_error("install_package failed");
  }

  const fs::path installed_solver = install_root / "lib" / "solvers" / "mini-solver";
  if (!fs::exists(installed_solver / "manifest.json")) {
    throw std::runtime_error("installed manifest missing");
  }
  if (!fs::exists(installed_solver / "bin" / "solver.txt")) {
    throw std::runtime_error("installed payload missing");
  }

  write_text(src_dir / "bin" / "solver.txt", "solver payload v2\n");
  if (!sattyre::create_bundle(src_dir.string(), bundle_path.string())) {
    throw std::runtime_error("create_bundle second pass failed");
  }

  if (sattyre::install_package(bundle_path.string(), install_root.string(), false) != sattyre::InstallerStatus::AlreadyExists) {
    throw std::runtime_error("install_package should reject existing destination without overwrite");
  }
  if (sattyre::install_package(bundle_path.string(), install_root.string(), true) != sattyre::InstallerStatus::Ok) {
    throw std::runtime_error("install_package overwrite failed");
  }

  if (sattyre::remove_package("mini-solver", install_root.string()) != sattyre::InstallerStatus::Ok) {
    throw std::runtime_error("remove_package failed");
  }
  if (fs::exists(installed_solver)) {
    throw std::runtime_error("package directory still exists after remove");
  }

  std::cout << "Package flow tests passed\n";
  return EXIT_SUCCESS;
}
