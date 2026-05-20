#include "sattyre/Registry.hpp"
#include "sattyre/Log.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>

#include "httplib.h"

namespace sattyre {
namespace {

bool parse_http_base(const std::string& base_url, std::string& host, int& port) {
  const std::string prefix = "http://";
  if (base_url.rfind(prefix, 0) != 0) {
    return false;
  }
  const std::string host_port = base_url.substr(prefix.size());
  const auto colon = host_port.find(':');
  if (colon == std::string::npos) {
    host = host_port;
    port = 80;
    return !host.empty();
  }
  host = host_port.substr(0, colon);
  try {
    port = std::stoi(host_port.substr(colon + 1));
  } catch (...) {
    return false;
  }
  return !host.empty() && port > 0;
}

}

RegistryStatus RegistryClient::deploy(const std::string& bundle_path, const std::string& registry_url) {
  log::info("Deploying bundle " + bundle_path + " to " + registry_url);
  namespace fs = std::filesystem;

  if (bundle_path.empty() || registry_url.empty()) {
    return RegistryStatus::InvalidArgument;
  }

  const fs::path source(bundle_path);
  const fs::path target_dir(registry_url);
  if (!fs::exists(source)) {
    return RegistryStatus::PackageNotFound;
  }
  std::error_code error;
  fs::create_directories(target_dir, error);
  if (error) {
    return RegistryStatus::IoError;
  }
  const fs::path target = target_dir / source.filename();
  fs::copy_file(source, target, fs::copy_options::overwrite_existing, error);
  return error ? RegistryStatus::IoError : RegistryStatus::Ok;
}

RegistryStatus RegistryClient::download(const std::string& package_name, const std::string& registry_url) {
  log::info("Downloading package " + package_name + " from " + registry_url);
  namespace fs = std::filesystem;

  if (package_name.empty() || registry_url.empty()) {
    return RegistryStatus::InvalidArgument;
  }

  const fs::path source = fs::path(registry_url) / package_name;
  if (!fs::exists(source)) {
    return RegistryStatus::PackageNotFound;
  }
  std::error_code error;
  fs::copy_file(source, fs::path(package_name), fs::copy_options::overwrite_existing, error);
  return error ? RegistryStatus::IoError : RegistryStatus::Ok;
}

RegistryStatus RegistryClient::list_http(const std::string& base_url, std::vector<std::string>& packages) {
  packages.clear();
  std::string host;
  int port = 0;
  if (!parse_http_base(base_url, host, port)) {
    return RegistryStatus::InvalidArgument;
  }

  httplib::Client client(host, port);
  auto response = client.Get("/packages");
  if (!response) {
    return RegistryStatus::IoError;
  }
  if (response->status != 200) {
    return RegistryStatus::IoError;
  }

  std::istringstream in(response->body);
  std::string line;
  while (std::getline(in, line)) {
    if (!line.empty()) {
      packages.push_back(line);
    }
  }
  return RegistryStatus::Ok;
}

RegistryStatus RegistryClient::deploy_http(const std::string& bundle_path, const std::string& base_url) {
  namespace fs = std::filesystem;
  const fs::path source(bundle_path);
  if (!fs::exists(source) || source.filename().empty()) {
    return RegistryStatus::PackageNotFound;
  }

  std::string host;
  int port = 0;
  if (!parse_http_base(base_url, host, port)) {
    return RegistryStatus::InvalidArgument;
  }

  std::ifstream in(source, std::ios::binary);
  if (!in) {
    return RegistryStatus::IoError;
  }
  std::string body((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

  httplib::Client client(host, port);
  const std::string path = "/packages/" + source.filename().string();
  auto response = client.Put(path.c_str(), body, "application/octet-stream");
  if (!response) {
    return RegistryStatus::IoError;
  }
  return (response->status == 201 || response->status == 200) ? RegistryStatus::Ok : RegistryStatus::IoError;
}

RegistryStatus RegistryClient::download_http(const std::string& package_name, const std::string& base_url) {
  if (package_name.empty()) {
    return RegistryStatus::InvalidArgument;
  }
  std::string host;
  int port = 0;
  if (!parse_http_base(base_url, host, port)) {
    return RegistryStatus::InvalidArgument;
  }

  httplib::Client client(host, port);
  auto response = client.Get(("/packages/" + package_name).c_str());
  if (!response) {
    return RegistryStatus::IoError;
  }
  if (response->status == 404) {
    return RegistryStatus::PackageNotFound;
  }
  if (response->status != 200) {
    return RegistryStatus::IoError;
  }

  std::ofstream out(package_name, std::ios::binary | std::ios::trunc);
  if (!out) {
    return RegistryStatus::IoError;
  }
  out.write(response->body.data(), static_cast<std::streamsize>(response->body.size()));
  return out.good() ? RegistryStatus::Ok : RegistryStatus::IoError;
}

} // namespace sattyre
