#include "sattyre/Sattyre-Plugin.hpp"

namespace sattyre {

bool plugin_abi_compatible(unsigned int abi_version) {
  return sattyre_plugin_abi_compatible(abi_version) == 1;
}

bool plugin_abi_compatible(const SattyrePluginInfo& info) {
  return plugin_abi_compatible(info.abi_version);
}

}
