#pragma once

#include <string>

namespace sattyre {

enum class InstallerStatus {
  Ok = 0,
  BundleNotFound,
  RootPathError,
  ExtractFailed,
  ManifestMissing,
  ManifestParseFailed,
  AlreadyExists,
  RemoveExistingFailed,
  CreateDestinationFailed,
  MoveFailed,
  RemoveFailed,
  NotFound
};

InstallerStatus install_package(const std::string& bundle_file, const std::string& root);
InstallerStatus install_package(const std::string& bundle_file, const std::string& root, bool overwrite);
InstallerStatus remove_package(const std::string& package_name, const std::string& root);

} // namespace sattyre
