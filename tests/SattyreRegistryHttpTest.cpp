#include "sattyre/Registry.hpp"

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <stdexcept>
#include <string>
#include <thread>
#include <vector>

#include "httplib.h"

int main() {
  namespace fs = std::filesystem;
  const fs::path temp_root = fs::temp_directory_path() / "sattyre-registry-http";
  const fs::path registry_root = temp_root / "registry";
  const fs::path client_root = temp_root / "client";
  const fs::path bundle_path = temp_root / "tiny.tar.gz";
  const int port = 18987;
  const std::string base_url = "http://127.0.0.1:18987";

  std::error_code ignore;
  fs::remove_all(temp_root, ignore);
  fs::create_directories(registry_root);
  fs::create_directories(client_root);

  {
    std::ofstream out(bundle_path, std::ios::binary);
    if (!out) throw std::runtime_error("failed to create bundle");
    out << "payload-http";
  }

  sattyre::RegistryServer server;
  std::atomic<bool> started{false};
  std::thread server_thread([&]() {
    started.store(true);
    (void)server.serve_http("127.0.0.1", port, registry_root.string());
  });
  server_thread.detach();

  while (!started.load()) {
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
  }
  bool healthy = false;
  for (int attempt = 0; attempt < 50; ++attempt) {
    httplib::Client probe("127.0.0.1", port);
    auto response = probe.Get("/health");
    if (response && response->status == 200) {
      healthy = true;
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(20));
  }
  if (!healthy) {
    throw std::runtime_error("registry server did not become healthy");
  }

  sattyre::RegistryClient client;
  if (client.deploy_http(bundle_path.string(), base_url) != sattyre::RegistryStatus::Ok) {
    throw std::runtime_error("deploy_http failed");
  }

  std::vector<std::string> packages;
  if (client.list_http(base_url, packages) != sattyre::RegistryStatus::Ok) {
    throw std::runtime_error("list_http failed");
  }
  if (packages.empty() || packages.front() != "tiny.tar.gz") {
    throw std::runtime_error("unexpected package list");
  }

  const fs::path old_cwd = fs::current_path();
  fs::current_path(client_root);
  if (client.download_http("tiny.tar.gz", base_url) != sattyre::RegistryStatus::Ok) {
    fs::current_path(old_cwd);
    throw std::runtime_error("download_http failed");
  }
  fs::current_path(old_cwd);

  if (!fs::exists(client_root / "tiny.tar.gz")) {
    throw std::runtime_error("downloaded file missing");
  }

  if (client.download_http("missing.tar.gz", base_url) != sattyre::RegistryStatus::PackageNotFound) {
    throw std::runtime_error("missing package status mismatch");
  }

  fs::remove_all(temp_root, ignore);
  return EXIT_SUCCESS;
}
