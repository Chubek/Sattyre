cmake_minimum_required(VERSION 3.20)
project(sattyre VERSION 0.1.0 LANGUAGES C CXX)

include(GNUInstallDirs)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SATTYRE_ENABLE_LUA "Enable Lua runtime and bindings" OFF)
option(SATTYRE_ENABLE_LOG4CPLUS "Enable log4cplus logging backend" OFF)
option(SATTYRE_ENABLE_REGISTRY "Enable registry components" ON)
option(SATTYRE_ENABLE_TESTS "Build tests" OFF)
option(SATTYRE_ENABLE_DOCS "Enable docs generation hooks" ON)
option(SATTYRE_ENABLE_LIBARCHIVE "Enable libarchive-backed package bundles" ON)
option(SATTYRE_ENABLE_SERDETK "Enable serdetk-backed manifest parsing" ON)
option(SATTYRE_ENABLE_REGISTRY_HTTP_TESTS "Enable live HTTP registry tests" OFF)
option(SATTYRE_INSTALL_TO_HOME "Install runtime artifacts to ~/.sattyre layout" ON)

if(SATTYRE_INSTALL_TO_HOME)
  set(SATTYRE_HOME_LAYOUT ".sattyre")
  set(CMAKE_INSTALL_BINDIR "${SATTYRE_HOME_LAYOUT}/bin")
  set(CMAKE_INSTALL_LIBDIR "${SATTYRE_HOME_LAYOUT}/lib")
  set(CMAKE_INSTALL_INCLUDEDIR "${SATTYRE_HOME_LAYOUT}/include")
endif()

add_library(sattyre STATIC
  src/core/SattyreSAT.cpp
  src/core/SattyreSMT.cpp
  src/solver/Sattyre-Solver.cpp
  src/solver/BruteForceSolver.cpp
  src/solver/DPLLSolver.cpp
  src/solver/CDCLSolver.cpp
  src/plugin/Sattyre-Plugin.cpp
  src/library/Sattyre-Library.cpp
  src/package/Manifest.cpp
  src/package/Bundle.cpp
  src/package/Installer.cpp
  src/util/Paths.cpp
  src/util/Log.cpp
  src/util/Dylib.cpp
)

if(SATTYRE_ENABLE_REGISTRY)
  target_include_directories(sattyre PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cpp-httplib)
  target_sources(sattyre PRIVATE
    src/registry/RegistryServer.cpp
    src/registry/RegistryClient.cpp
  )
endif()

if(SATTYRE_ENABLE_LUA)
  target_sources(sattyre PRIVATE
    src/lua/LuaRuntime.cpp
    src/lua/LuaBindings.cpp
  )
endif()

target_include_directories(sattyre
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

if(SATTYRE_ENABLE_SERDETK)
  target_include_directories(sattyre PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/serdetk)
  target_compile_definitions(sattyre PRIVATE SATTYRE_HAVE_SERDETK=1)
endif()

target_compile_definitions(sattyre PRIVATE
  $<$<BOOL:${SATTYRE_ENABLE_LOG4CPLUS}>:SATTYRE_USE_LOG4CPLUS>
)

if(SATTYRE_ENABLE_LIBARCHIVE)
  find_package(LibArchive QUIET)
  if(NOT LibArchive_FOUND AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libarchive/CMakeLists.txt)
    set(ENABLE_TEST OFF CACHE BOOL "Disable libarchive upstream tests" FORCE)
    set(ENABLE_CPIO OFF CACHE BOOL "Disable cpio tool build" FORCE)
    set(ENABLE_TAR OFF CACHE BOOL "Disable tar tool build" FORCE)
    set(ENABLE_CAT OFF CACHE BOOL "Disable cat tool build" FORCE)
    set(ENABLE_UNZIP OFF CACHE BOOL "Disable unzip tool build" FORCE)
    set(ENABLE_INSTALL OFF CACHE BOOL "Disable libarchive install rules" FORCE)
    add_subdirectory(third_party/libarchive EXCLUDE_FROM_ALL)
  endif()
  if(LibArchive_FOUND)
    target_link_libraries(sattyre PRIVATE LibArchive::LibArchive)
    target_compile_definitions(sattyre PRIVATE SATTYRE_HAVE_LIBARCHIVE=1)
  elseif(TARGET archive_static)
    target_link_libraries(sattyre PRIVATE archive_static)
    target_include_directories(sattyre PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libarchive/libarchive)
    target_compile_definitions(sattyre PRIVATE SATTYRE_HAVE_LIBARCHIVE=1)
  elseif(TARGET archive)
    target_link_libraries(sattyre PRIVATE archive)
    target_include_directories(sattyre PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/libarchive/libarchive)
    target_compile_definitions(sattyre PRIVATE SATTYRE_HAVE_LIBARCHIVE=1)
  endif()
endif()

if(SATTYRE_ENABLE_LUA)
  find_package(Lua REQUIRED)
  target_include_directories(sattyre PRIVATE ${LUA_INCLUDE_DIR})
  target_link_libraries(sattyre PRIVATE ${LUA_LIBRARIES})
endif()

if(UNIX AND NOT APPLE)
  target_link_libraries(sattyre PRIVATE dl)
endif()

add_executable(sattyre-cli src/cli/sattyre-cli.cpp)
target_link_libraries(sattyre-cli PRIVATE sattyre)

add_executable(sattyre-packman src/cli/sattyre-packman.cpp)
target_link_libraries(sattyre-packman PRIVATE sattyre)

if(SATTYRE_ENABLE_REGISTRY)
  add_executable(sattyre-registry src/cli/sattyre-registry.cpp)
  target_link_libraries(sattyre-registry PRIVATE sattyre)
endif()

add_library(sattyre-cabi STATIC
  src/solver/Sattyre-Solver.c
  src/plugin/Sattyre-Plugin.c
  src/library/Sattyre-Library.c
)
target_include_directories(sattyre-cabi PRIVATE include)
target_link_libraries(sattyre PRIVATE sattyre-cabi)

configure_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sattyre.pc.in
  ${CMAKE_CURRENT_BINARY_DIR}/sattyre.pc
  @ONLY
)

install(TARGETS sattyre sattyre-cli sattyre-packman
  EXPORT sattyreTargets
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(TARGET sattyre-registry)
  install(TARGETS sattyre-registry
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )
endif()

install(DIRECTORY include/sattyre DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/sattyre.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
install(PROGRAMS
  scripts/pkg-bootstrap.pl
  scripts/pkg-archive.pl
  scripts/pkg-install.pl
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(DIRECTORY examples/ DESTINATION .sattyre/examples OPTIONAL)
install(DIRECTORY exmpls/packages/ DESTINATION .sattyre/share/packages OPTIONAL)

if(SATTYRE_ENABLE_DOCS)
  find_package(Doxygen QUIET)
  if(DOXYGEN_FOUND)
    include(${CMAKE_CURRENT_SOURCE_DIR}/docs/CMakeFiles.txt)
  else()
    message(STATUS "Doxygen not found; documentation generation is disabled")
  endif()
endif()

if(SATTYRE_ENABLE_TESTS)
  enable_testing()
  add_executable(sattyre-sat-parser-test tests/SattyreSATParserTest.cpp)
  target_link_libraries(sattyre-sat-parser-test PRIVATE sattyre)
  add_test(NAME sattyre-sat-parser-test COMMAND sattyre-sat-parser-test)

  add_executable(sattyre-solver-abi-test tests/SattyreSolverAbiTest.cpp)
  target_link_libraries(sattyre-solver-abi-test PRIVATE sattyre sattyre-cabi)
  add_test(NAME sattyre-solver-abi-test COMMAND sattyre-solver-abi-test)

  add_executable(sattyre-module-abi-test tests/SattyreModuleAbiTest.cpp)
  target_link_libraries(sattyre-module-abi-test PRIVATE sattyre sattyre-cabi)
  add_test(NAME sattyre-module-abi-test COMMAND sattyre-module-abi-test)

  add_executable(sattyre-module-cpp-abi-test tests/SattyreModuleCppAbiTest.cpp)
  target_link_libraries(sattyre-module-cpp-abi-test PRIVATE sattyre sattyre-cabi)
  add_test(NAME sattyre-module-cpp-abi-test COMMAND sattyre-module-cpp-abi-test)

  add_executable(sattyre-manifest-test tests/SattyreManifestTest.cpp)
  target_link_libraries(sattyre-manifest-test PRIVATE sattyre)
  add_test(NAME sattyre-manifest-test COMMAND sattyre-manifest-test)

  add_executable(sattyre-package-flow-test tests/SattyrePackageFlowTest.cpp)
  target_link_libraries(sattyre-package-flow-test PRIVATE sattyre)
  add_test(NAME sattyre-package-flow-test COMMAND sattyre-package-flow-test)

  add_executable(sattyre-cli-smoke-test tests/SattyreCliSmokeTest.cpp)
  target_link_libraries(sattyre-cli-smoke-test PRIVATE sattyre)
  add_test(NAME sattyre-cli-smoke-test COMMAND $<TARGET_FILE:sattyre-cli-smoke-test>)
  set_tests_properties(sattyre-cli-smoke-test PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  add_executable(sattyre-packman-cli-test tests/SattyrePackmanCliTest.cpp)
  add_dependencies(sattyre-packman-cli-test sattyre-packman)
  add_test(NAME sattyre-packman-cli-test COMMAND $<TARGET_FILE:sattyre-packman-cli-test>)
  set_tests_properties(sattyre-packman-cli-test PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

  if(SATTYRE_ENABLE_REGISTRY)
    add_executable(sattyre-registry-flow-test tests/SattyreRegistryFlowTest.cpp)
    target_link_libraries(sattyre-registry-flow-test PRIVATE sattyre)
    add_test(NAME sattyre-registry-flow-test COMMAND sattyre-registry-flow-test)

    add_executable(sattyre-registry-cli-test tests/SattyreRegistryCliTest.cpp)
    add_dependencies(sattyre-registry-cli-test sattyre-registry)
    add_test(NAME sattyre-registry-cli-test COMMAND $<TARGET_FILE:sattyre-registry-cli-test>)
    set_tests_properties(sattyre-registry-cli-test PROPERTIES WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

    if(SATTYRE_ENABLE_REGISTRY_HTTP_TESTS)
      add_executable(sattyre-registry-http-test tests/SattyreRegistryHttpTest.cpp)
      target_include_directories(sattyre-registry-http-test PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/third_party/cpp-httplib)
      target_link_libraries(sattyre-registry-http-test PRIVATE sattyre)
      add_test(NAME sattyre-registry-http-test COMMAND sattyre-registry-http-test)
    endif()
  endif()

  if(SATTYRE_ENABLE_LUA)
    add_executable(sattyre-lua-smoke-test tests/SattyreLuaSmokeTest.cpp)
    target_include_directories(sattyre-lua-smoke-test PRIVATE ${LUA_INCLUDE_DIR})
    target_link_libraries(sattyre-lua-smoke-test PRIVATE sattyre ${LUA_LIBRARIES})
    add_test(NAME sattyre-lua-smoke-test COMMAND sattyre-lua-smoke-test)
  endif()
endif()
