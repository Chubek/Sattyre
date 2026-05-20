#pragma once

#include <vector>
#include <string>

namespace sattyre {

enum class RegistryStatus {
  Ok = 0,
  RegistryNotFound,
  PackageNotFound,
  IoError,
  InvalidArgument
};

class RegistryServer {
public:
  RegistryStatus serve(const std::string& host, int port);
  RegistryStatus list_packages(const std::string& registry_root, std::vector<std::string>& packages) const;
  RegistryStatus serve_http(const std::string& host, int port, const std::string& registry_root);
};

class RegistryClient {
public:
  RegistryStatus deploy(const std::string& bundle_path, const std::string& registry_url);
  RegistryStatus download(const std::string& package_name, const std::string& registry_url);
  RegistryStatus list_http(const std::string& base_url, std::vector<std::string>& packages);
  RegistryStatus deploy_http(const std::string& bundle_path, const std::string& base_url);
  RegistryStatus download_http(const std::string& package_name, const std::string& base_url);
};

} // namespace sattyre
