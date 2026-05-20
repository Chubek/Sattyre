#ifndef SATTYRE_LIBRARY_H
#define SATTYRE_LIBRARY_H

#ifdef __cplusplus
extern "C" {
#endif

#define SATTYRE_LIBRARY_ABI_VERSION 1u

typedef struct {
  unsigned int abi_version;
  const char* name;
  const char* version;
} SattyreLibraryInfo;

SattyreLibraryInfo sattyre_library_info(void);
int sattyre_library_abi_compatible(unsigned int abi_version);

#ifdef __cplusplus
}
#endif

#endif
