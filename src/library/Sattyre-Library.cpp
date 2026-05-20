#include "sattyre/Sattyre-Library.hpp"

namespace sattyre {

bool library_abi_compatible(unsigned int abi_version) {
  return sattyre_library_abi_compatible(abi_version) == 1;
}

bool library_abi_compatible(const SattyreLibraryInfo& info) {
  return library_abi_compatible(info.abi_version);
}

}
