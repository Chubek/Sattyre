#include "sattyre/Bundle.hpp"
#include "sattyre/Installer.hpp"
#include "sattyre/Log.hpp"
#include "sattyre/Manifest.hpp"
#include "sattyre/Version.hpp"

#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace {

void print_usage() {
  std::cout << "sattyre-packman " << SATTYRE_VERSION_STRING << "\n";
  std::cout << "usage:\n";
  std::cout << "  sattyre-packman validate <manifest-or-package-dir>\n";
  std::cout << "  sattyre-packman pack <source-dir> <bundle-file>\n";
  std::cout << "  sattyre-packman unpack <bundle-file> <out-dir>\n";
  std::cout << "  sattyre-packman install <bundle-file> [--root PATH] [--overwrite]\n";
  std::cout << "  sattyre-packman remove <package-name> [--root PATH]\n";
}

std::filesystem::path resolve_manifest_path(const std::filesystem::path& path) {
  namespace fs = std::filesystem;
  if (fs::is_regular_file(path)) {
    return path;
  }
  if (!fs::is_directory(path)) {
    return {};
  }
  for (const auto& candidate : {
         path / "manifest.json",
         path / "manifest.yaml",
         path / "manifest.yml",
         path / "manifest.xml",
         path / "manifest.sexp",
         path / "manifest.scm",
         path / "manifest.lisp"}) {
    if (fs::exists(candidate) && fs::is_regular_file(candidate)) {
      return candidate;
    }
  }
  return {};
}

int exit_with_error(const std::string& message) {
  std::cerr << "sattyre-packman: " << message << "\n";
  return 2;
}

std::string installer_status_message(sattyre::InstallerStatus status) {
  switch (status) {
    case sattyre::InstallerStatus::Ok: return "ok";
    case sattyre::InstallerStatus::BundleNotFound: return "bundle not found";
    case sattyre::InstallerStatus::RootPathError: return "invalid root path";
    case sattyre::InstallerStatus::ExtractFailed: return "bundle extraction failed";
    case sattyre::InstallerStatus::ManifestMissing: return "manifest missing";
    case sattyre::InstallerStatus::ManifestParseFailed: return "manifest parse failed";
    case sattyre::InstallerStatus::AlreadyExists: return "package already exists";
    case sattyre::InstallerStatus::RemoveExistingFailed: return "failed to remove existing installation";
    case sattyre::InstallerStatus::CreateDestinationFailed: return "failed to create destination";
    case sattyre::InstallerStatus::MoveFailed: return "failed to move staged package";
    case sattyre::InstallerStatus::RemoveFailed: return "remove failed";
    case sattyre::InstallerStatus::NotFound: return "not found";
  }
  return "unknown error";
}

} // namespace

int main(int argc, char** argv) {
  sattyre::log::init();
  sattyre::log::info("sattyre-packman starting");

  if (argc < 2) {
    print_usage();
    return 1;
  }

  const std::string command = argv[1];
  if (command == "validate") {
    if (argc != 3) {
      return print_usage(), 1;
    }
    const std::filesystem::path manifest_path = resolve_manifest_path(argv[2]);
    if (manifest_path.empty()) {
      return exit_with_error("manifest not found");
    }
    try {
      const auto manifest = sattyre::load_manifest(manifest_path.string());
      std::cout << manifest.name << " " << manifest.version << " " << manifest.type << "\n";
      return 0;
    } catch (const std::exception& error) {
      return exit_with_error(error.what());
    }
  }

  if (command == "pack") {
    if (argc != 4) {
      return print_usage(), 1;
    }
    if (!sattyre::create_bundle(argv[2], argv[3])) {
      return exit_with_error("pack failed");
    }
    return 0;
  }

  if (command == "unpack") {
    if (argc != 4) {
      return print_usage(), 1;
    }
    if (!sattyre::extract_bundle(argv[2], argv[3])) {
      return exit_with_error("unpack failed");
    }
    return 0;
  }

  if (command == "install") {
    std::string root;
    bool overwrite = false;
    std::vector<std::string> positional;
    for (int index = 2; index < argc; ++index) {
      const std::string arg = argv[index];
      if (arg == "--root" && index + 1 < argc) {
        root = argv[++index];
      } else if (arg == "--overwrite") {
        overwrite = true;
      } else if (!arg.empty() && arg.front() == '-') {
        return print_usage(), 1;
      } else {
        positional.push_back(arg);
      }
    }
    if (positional.size() != 1) {
      return print_usage(), 1;
    }
    const auto status = sattyre::install_package(positional[0], root, overwrite);
    if (status != sattyre::InstallerStatus::Ok) {
      return exit_with_error(installer_status_message(status));
    }
    return 0;
  }

  if (command == "remove") {
    std::string root;
    std::vector<std::string> positional;
    for (int index = 2; index < argc; ++index) {
      const std::string arg = argv[index];
      if (arg == "--root" && index + 1 < argc) {
        root = argv[++index];
      } else if (!arg.empty() && arg.front() == '-') {
        return print_usage(), 1;
      } else {
        positional.push_back(arg);
      }
    }
    if (positional.size() != 1) {
      return print_usage(), 1;
    }
    const auto status = sattyre::remove_package(positional[0], root);
    if (status != sattyre::InstallerStatus::Ok) {
      return exit_with_error(installer_status_message(status));
    }
    return 0;
  }

  print_usage();
  return 1;
}
