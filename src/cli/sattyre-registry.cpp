#include "sattyre/Log.hpp"
#include "sattyre/Registry.hpp"
#include "sattyre/Version.hpp"

#include <cstdlib>
#include <iostream>
#include <string>

namespace {

void print_usage() {
  std::cout << "sattyre-registry " << SATTYRE_VERSION_STRING << "\n";
  std::cout << "usage: sattyre-registry serve [--host HOST] [--port PORT] [--root PATH]\n";
}

}

int main(int argc, char** argv) {
  sattyre::log::init();
  sattyre::log::info("sattyre-registry starting");

  if (argc < 2) {
    print_usage();
    return 1;
  }

  const std::string command = argv[1];
  if (command != "serve") {
    print_usage();
    return 1;
  }

  std::string host = "127.0.0.1";
  int port = 8080;
  std::string root = "./registry-root";
  for (int index = 2; index < argc; ++index) {
    const std::string arg = argv[index];
    if (arg == "--host" && index + 1 < argc) {
      host = argv[++index];
    } else if (arg == "--port" && index + 1 < argc) {
      port = std::atoi(argv[++index]);
    } else if (arg == "--root" && index + 1 < argc) {
      root = argv[++index];
    } else {
      print_usage();
      return 1;
    }
  }

  sattyre::RegistryServer server;
  const sattyre::RegistryStatus status = server.serve_http(host, port, root);
  if (status != sattyre::RegistryStatus::Ok) {
    std::cerr << "failed to start registry server\n";
    return 2;
  }
  return 0;
}
