#ifndef SATTYRE_PLUGIN_H
#define SATTYRE_PLUGIN_H

#ifdef __cplusplus
extern "C" {
#endif

#define SATTYRE_PLUGIN_ABI_VERSION 1u

typedef struct {
  unsigned int abi_version;
  const char* name;
  const char* version;
} SattyrePluginInfo;

int sattyre_plugin_init(void);
void sattyre_plugin_shutdown(void);
SattyrePluginInfo sattyre_plugin_info(void);
int sattyre_plugin_abi_compatible(unsigned int abi_version);

#ifdef __cplusplus
}
#endif

#endif
