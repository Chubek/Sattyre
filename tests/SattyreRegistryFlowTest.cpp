#include "sattyre/Registry.hpp"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

int main() {
  namespace fs = std::filesystem;
  const fs::path temp_root = fs::temp_directory_path() / "sattyre-registry-flow";
  const fs::path local_registry = temp_root / "registry";
  const fs::path local_download = temp_root / "download";
  const fs::path bundle_path = temp_root / "mini-solver.tar.gz";

  std::error_code ignore;
  fs::remove_all(temp_root, ignore);
  fs::create_directories(temp_root);
  fs::create_directories(local_download);

  {
    std::ofstream out(bundle_path, std::ios::binary);
    if (!out) {
      throw std::runtime_error("failed to create test bundle");
    }
    out << "fake bundle payload";
  }

  sattyre::RegistryClient client;
  sattyre::RegistryServer server;

  if (client.deploy(bundle_path.string(), local_registry.string()) != sattyre::RegistryStatus::Ok) {
    throw std::runtime_error("deploy failed");
  }

  std::vector<std::string> packages;
  if (server.list_packages(local_registry.string(), packages) != sattyre::RegistryStatus::Ok) {
    throw std::runtime_error("list_packages failed");
  }
  if (packages.empty() || packages.front() != "mini-solver.tar.gz") {
    throw std::runtime_error("unexpected package listing");
  }

  const fs::path old_cwd = fs::current_path();
  fs::current_path(local_download);
  if (client.download("mini-solver.tar.gz", local_registry.string()) != sattyre::RegistryStatus::Ok) {
    fs::current_path(old_cwd);
    throw std::runtime_error("download failed");
  }
  fs::current_path(old_cwd);

  if (!fs::exists(local_download / "mini-solver.tar.gz")) {
    throw std::runtime_error("downloaded file missing");
  }

  if (client.download("missing.tar.gz", local_registry.string()) != sattyre::RegistryStatus::PackageNotFound) {
    throw std::runtime_error("missing package status mismatch");
  }

  fs::remove_all(temp_root, ignore);
  return EXIT_SUCCESS;
}
