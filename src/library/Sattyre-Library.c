#include "sattyre/Sattyre-Library.h"

SattyreLibraryInfo sattyre_library_info(void) {
  SattyreLibraryInfo info;
  info.abi_version = SATTYRE_LIBRARY_ABI_VERSION;
  info.name = "SattyreLibrary";
  info.version = "0.1.0";
  return info;
}

int sattyre_library_abi_compatible(unsigned int abi_version) {
  return abi_version == SATTYRE_LIBRARY_ABI_VERSION;
}
