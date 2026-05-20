#include "sattyre/Sattyre-Library.hpp"
#include "sattyre/Sattyre-Plugin.hpp"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  const SattyrePluginInfo plugin = sattyre_plugin_info();
  const SattyreLibraryInfo library = sattyre_library_info();

  if (!sattyre::plugin_abi_compatible(plugin)) {
    throw std::runtime_error("C++ plugin ABI helper rejected plugin info");
  }
  if (!sattyre::library_abi_compatible(library)) {
    throw std::runtime_error("C++ library ABI helper rejected library info");
  }
  if (!sattyre::plugin_abi_compatible(SATTYRE_PLUGIN_ABI_VERSION)) {
    throw std::runtime_error("C++ plugin ABI helper rejected current version");
  }
  if (!sattyre::library_abi_compatible(SATTYRE_LIBRARY_ABI_VERSION)) {
    throw std::runtime_error("C++ library ABI helper rejected current version");
  }

  std::cout << "C++ module ABI compatibility tests passed\n";
  return EXIT_SUCCESS;
}
