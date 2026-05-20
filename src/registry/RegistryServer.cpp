#include "sattyre/Registry.hpp"
#include "sattyre/Log.hpp"

#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

#include "httplib.h"

namespace sattyre {

RegistryStatus RegistryServer::serve(const std::string& host, int port) {
  log::info("Starting registry server on " + host + ":" + std::to_string(port));

  const std::filesystem::path index = std::filesystem::path("registry-index.txt");
  std::ofstream out(index, std::ios::app);
  return static_cast<bool>(out) ? RegistryStatus::Ok : RegistryStatus::IoError;
}

RegistryStatus RegistryServer::list_packages(const std::string& registry_root, std::vector<std::string>& packages) const {
  namespace fs = std::filesystem;
  packages.clear();

  const fs::path root(registry_root);
  if (registry_root.empty()) {
    return RegistryStatus::InvalidArgument;
  }
  if (!fs::exists(root) || !fs::is_directory(root)) {
    return RegistryStatus::RegistryNotFound;
  }

  std::error_code error;
  for (const auto& entry : fs::directory_iterator(root, error)) {
    if (error) {
      return RegistryStatus::IoError;
    }
    if (entry.is_regular_file(error) && !error) {
      packages.push_back(entry.path().filename().string());
    }
  }
  return RegistryStatus::Ok;
}

RegistryStatus RegistryServer::serve_http(const std::string& host, int port, const std::string& registry_root) {
  namespace fs = std::filesystem;
  if (host.empty() || port <= 0 || registry_root.empty()) {
    return RegistryStatus::InvalidArgument;
  }

  const fs::path root(registry_root);
  std::error_code error;
  fs::create_directories(root, error);
  if (error) {
    return RegistryStatus::IoError;
  }

  httplib::Server server;

  server.Get("/health", [](const httplib::Request&, httplib::Response& response) {
    response.set_content("ok\n", "text/plain");
  });

  server.Get("/packages", [this, registry_root](const httplib::Request&, httplib::Response& response) {
    std::vector<std::string> packages;
    const RegistryStatus status = list_packages(registry_root, packages);
    if (status != RegistryStatus::Ok) {
      response.status = 500;
      response.set_content("list failed\n", "text/plain");
      return;
    }
    std::ostringstream out;
    for (const auto& package : packages) {
      out << package << "\n";
    }
    response.set_content(out.str(), "text/plain");
  });

  server.Get(R"(/packages/(.+))", [registry_root](const httplib::Request& request, httplib::Response& response) {
    namespace fs = std::filesystem;
    const fs::path file_name(request.matches[1].str());
    if (file_name.filename() != file_name) {
      response.status = 400;
      response.set_content("invalid package name\n", "text/plain");
      return;
    }
    const fs::path package_path = fs::path(registry_root) / file_name;
    std::ifstream in(package_path, std::ios::binary);
    if (!in) {
      response.status = 404;
      response.set_content("not found\n", "text/plain");
      return;
    }
    std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
    response.set_content(content, "application/octet-stream");
  });

  server.Put(R"(/packages/(.+))", [registry_root](const httplib::Request& request, httplib::Response& response) {
    namespace fs = std::filesystem;
    const fs::path file_name(request.matches[1].str());
    if (file_name.filename() != file_name) {
      response.status = 400;
      response.set_content("invalid package name\n", "text/plain");
      return;
    }
    const fs::path package_path = fs::path(registry_root) / file_name;
    std::ofstream out(package_path, std::ios::binary | std::ios::trunc);
    if (!out) {
      response.status = 500;
      response.set_content("write failed\n", "text/plain");
      return;
    }
    out.write(request.body.data(), static_cast<std::streamsize>(request.body.size()));
    if (!out.good()) {
      response.status = 500;
      response.set_content("write failed\n", "text/plain");
      return;
    }
    response.status = 201;
    response.set_content("stored\n", "text/plain");
  });

  if (!server.listen(host, port)) {
    return RegistryStatus::IoError;
  }
  return RegistryStatus::Ok;
}

} // namespace sattyre
