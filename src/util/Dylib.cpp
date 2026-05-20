#include "sattyre/Dylib.hpp"

#if __has_include(<dynalo/dynalo.hpp>)
#  include <dynalo/dynalo.hpp>
#  define SATTYRE_HAS_DYNALO 1
#else
#  define SATTYRE_HAS_DYNALO 0
#endif

#if !SATTYRE_HAS_DYNALO
#  if defined(_WIN32)
#    include <windows.h>
#  else
#    include <dlfcn.h>
#  endif
#endif

#include <stdexcept>

namespace sattyre {

struct DynamicLibrary::Impl {
#if SATTYRE_HAS_DYNALO
  dynalo::native::handle handle = dynalo::native::invalid_handle();
#else
  void* handle = nullptr;
#endif
};

DynamicLibrary::DynamicLibrary() : impl_(new Impl{}) {}
DynamicLibrary::~DynamicLibrary() {
  close();
  delete impl_;
  impl_ = nullptr;
}

bool DynamicLibrary::open(const std::string& path) {
  close();
#if SATTYRE_HAS_DYNALO
  try {
    impl_->handle = dynalo::open(path);
    return true;
  } catch (const std::runtime_error&) {
    impl_->handle = dynalo::native::invalid_handle();
    return false;
  }
#else
#  if defined(_WIN32)
  impl_->handle = (void*)LoadLibraryA(path.c_str());
#  else
  impl_->handle = dlopen(path.c_str(), RTLD_NOW);
#  endif
  return impl_->handle != nullptr;
#endif
}

void DynamicLibrary::close() {
  if (!impl_) return;
#if SATTYRE_HAS_DYNALO
  if (impl_->handle == dynalo::native::invalid_handle()) return;
  try {
    dynalo::close(impl_->handle);
  } catch (const std::runtime_error&) {
  }
  impl_->handle = dynalo::native::invalid_handle();
#else
  if (!impl_->handle) return;
#  if defined(_WIN32)
  FreeLibrary((HMODULE)impl_->handle);
#  else
  dlclose(impl_->handle);
#  endif
  impl_->handle = nullptr;
#endif
}

void* DynamicLibrary::symbol(const std::string& name) const {
  if (!impl_) return nullptr;
#if SATTYRE_HAS_DYNALO
  if (impl_->handle == dynalo::native::invalid_handle()) return nullptr;
  try {
    return reinterpret_cast<void*>(dynalo::get_function<void()>(impl_->handle, name));
  } catch (const std::runtime_error&) {
    return nullptr;
  }
#else
  if (!impl_->handle) return nullptr;
#  if defined(_WIN32)
  return (void*)GetProcAddress((HMODULE)impl_->handle, name.c_str());
#  else
  return dlsym(impl_->handle, name.c_str());
#  endif
#endif
}

bool DynamicLibrary::loaded() const {
  if (!impl_) return false;
#if SATTYRE_HAS_DYNALO
  return impl_->handle != dynalo::native::invalid_handle();
#else
  return impl_->handle != nullptr;
#endif
}

} // namespace sattyre
