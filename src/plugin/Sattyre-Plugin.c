#include "sattyre/Sattyre-Plugin.h"

int sattyre_plugin_init(void) {
  return 0;
}

void sattyre_plugin_shutdown(void) {
}

SattyrePluginInfo sattyre_plugin_info(void) {
  SattyrePluginInfo info;
  info.abi_version = SATTYRE_PLUGIN_ABI_VERSION;
  info.name = "SattyrePlugin";
  info.version = "0.1.0";
  return info;
}

int sattyre_plugin_abi_compatible(unsigned int abi_version) {
  return abi_version == SATTYRE_PLUGIN_ABI_VERSION;
}
