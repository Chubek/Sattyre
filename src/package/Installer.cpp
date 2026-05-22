#include "sattyre/Installer.hpp"
#include "sattyre/Bundle.hpp"
#include "sattyre/Log.hpp"
#include "sattyre/Manifest.hpp"
#include "sattyre/Paths.hpp"

#include <filesystem>

namespace sattyre {

InstallerStatus install_package(const std::string& bundle_file, const std::string& root) {
  return install_package(bundle_file, root, true);
}

InstallerStatus install_package(const std::string& bundle_file, const std::string& root, bool overwrite) {
  log::info("Installing package " + bundle_file + " into " + root);

  namespace fs = std::filesystem;
  const fs::path bundle_path(bundle_file);
  if (!fs::exists(bundle_path)) {
    return InstallerStatus::BundleNotFound;
  }

  fs::path root_path;
  try {
    root_path = fs::weakly_canonical(root.empty() ? sattyre::paths::home() : fs::path(root));
  } catch (...) {
    return InstallerStatus::RootPathError;
  }
  const fs::path staging_dir = root_path / ".sattyre-install-staging";
  fs::remove_all(staging_dir);
  fs::create_directories(staging_dir);
  if (!extract_bundle(bundle_file, staging_dir.string())) {
    return InstallerStatus::ExtractFailed;
  }

  fs::path manifest_path;
  for (const auto& candidate : {
         staging_dir / "manifest.json",
         staging_dir / "manifest.yaml",
         staging_dir / "manifest.yml",
         staging_dir / "manifest.xml",
         staging_dir / "manifest.sexp",
         staging_dir / "manifest.scm",
         staging_dir / "manifest.lisp"}) {
    if (fs::exists(candidate) && fs::is_regular_file(candidate)) {
      manifest_path = candidate;
      break;
    }
  }
  if (manifest_path.empty()) {
    std::error_code ignore;
    fs::remove_all(staging_dir, ignore);
    return InstallerStatus::ManifestMissing;
  }

  PackageManifest manifest;
  try {
    manifest = load_manifest(manifest_path.string());
  } catch (...) {
    std::error_code ignore;
    fs::remove_all(staging_dir, ignore);
    return InstallerStatus::ManifestParseFailed;
  }

  fs::path destination_base = root_path;
  if (manifest.type == "solver") {
    destination_base = root_path / "lib" / "solvers";
  } else if (manifest.type == "plugin") {
    destination_base = root_path / "lib" / "plugins";
  } else if (manifest.type == "library") {
    destination_base = root_path / "lib";
  }

  const fs::path destination = destination_base / manifest.name;
  std::error_code error;
  if (fs::exists(destination)) {
    if (!overwrite) {
      fs::remove_all(staging_dir, error);
      return InstallerStatus::AlreadyExists;
    }
    fs::remove_all(destination, error);
    if (error) {
      fs::remove_all(staging_dir, error);
      return InstallerStatus::RemoveExistingFailed;
    }
  }
  fs::create_directories(destination_base, error);
  if (error) {
    fs::remove_all(staging_dir, error);
    return InstallerStatus::CreateDestinationFailed;
  }
  fs::rename(staging_dir, destination, error);
  if (error) {
    fs::remove_all(staging_dir, error);
    return InstallerStatus::MoveFailed;
  }
  return InstallerStatus::Ok;
}

InstallerStatus remove_package(const std::string& package_name, const std::string& root) {
  log::warn("Removing package " + package_name + " from " + root);

  namespace fs = std::filesystem;
  const fs::path root_path(root.empty() ? sattyre::paths::home() : fs::path(root));
  const fs::path solver_path = root_path / "lib" / "solvers" / package_name;
  const fs::path plugin_path = root_path / "lib" / "plugins" / package_name;
  const fs::path library_path = root_path / "lib" / package_name;

  bool removed = false;
  if (fs::exists(solver_path)) {
    removed = fs::remove_all(solver_path) > 0 || removed;
  }
  if (fs::exists(plugin_path)) {
    removed = fs::remove_all(plugin_path) > 0 || removed;
  }
  if (fs::exists(library_path)) {
    removed = fs::remove_all(library_path) > 0 || removed;
  }
  return removed ? InstallerStatus::Ok : InstallerStatus::NotFound;
}

} // namespace sattyre
