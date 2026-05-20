#include "sattyre/Sattyre-Library.h"
#include "sattyre/Sattyre-Plugin.h"

#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main() {
  const SattyrePluginInfo plugin = sattyre_plugin_info();
  const SattyreLibraryInfo library = sattyre_library_info();

  if (sattyre_plugin_abi_compatible(SATTYRE_PLUGIN_ABI_VERSION) != 1) {
    throw std::runtime_error("plugin ABI should be compatible");
  }
  if (sattyre_plugin_abi_compatible(plugin.abi_version) != 1) {
    throw std::runtime_error("plugin info ABI should be compatible");
  }
  if (sattyre_plugin_abi_compatible(SATTYRE_PLUGIN_ABI_VERSION + 1u) != 0) {
    throw std::runtime_error("plugin ABI should reject newer version");
  }

  if (sattyre_library_abi_compatible(SATTYRE_LIBRARY_ABI_VERSION) != 1) {
    throw std::runtime_error("library ABI should be compatible");
  }
  if (sattyre_library_abi_compatible(library.abi_version) != 1) {
    throw std::runtime_error("library info ABI should be compatible");
  }
  if (sattyre_library_abi_compatible(SATTYRE_LIBRARY_ABI_VERSION + 1u) != 0) {
    throw std::runtime_error("library ABI should reject newer version");
  }

  std::cout << "Module ABI compatibility tests passed\n";
  return EXIT_SUCCESS;
}
