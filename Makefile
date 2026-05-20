CXX ?= c++
CC  ?= cc

CXXFLAGS ?= -std=c++20 -Iinclude -Wall -Wextra -Wpedantic -O2
CFLAGS   ?= -Iinclude -Wall -Wextra -Wpedantic -O2

BUILD_DIR := build

COMMON_CPP := \
src/core/SattyreSAT.cpp \
src/core/SattyreSMT.cpp \
src/solver/Sattyre-Solver.cpp \
src/plugin/Sattyre-Plugin.cpp \
src/library/Sattyre-Library.cpp \
src/package/Manifest.cpp \
src/package/Bundle.cpp \
src/package/Installer.cpp \
src/registry/RegistryServer.cpp \
src/registry/RegistryClient.cpp \
src/lua/LuaRuntime.cpp \
src/lua/LuaBindings.cpp \
src/util/Paths.cpp \
src/util/Log.cpp \
src/util/Dylib.cpp

CLI_SRC := src/cli/sattyre-cli.cpp
PACKMAN_SRC := src/cli/sattyre-packman.cpp
REGISTRY_SRC := src/cli/sattyre-registry.cpp

C_ABI_SRC := \
src/solver/Sattyre-Solver.c \
src/plugin/Sattyre-Plugin.c \
src/library/Sattyre-Library.c

all: dirs sattyre-cli sattyre-packman sattyre-registry c-abi

dirs:
mkdir -p $(BUILD_DIR)

sattyre-cli:
$(CXX) $(CXXFLAGS) $(CLI_SRC) $(COMMON_CPP) -o $(BUILD_DIR)/sattyre-cli

sattyre-packman:
$(CXX) $(CXXFLAGS) $(PACKMAN_SRC) $(COMMON_CPP) -o $(BUILD_DIR)/sattyre-packman

sattyre-registry:
$(CXX) $(CXXFLAGS) $(REGISTRY_SRC) $(COMMON_CPP) -o $(BUILD_DIR)/sattyre-registry

c-abi:
$(CC) $(CFLAGS) -c src/solver/Sattyre-Solver.c -o $(BUILD_DIR)/Sattyre-Solver.o
$(CC) $(CFLAGS) -c src/plugin/Sattyre-Plugin.c -o $(BUILD_DIR)/Sattyre-Plugin.o
$(CC) $(CFLAGS) -c src/library/Sattyre-Library.c -o $(BUILD_DIR)/Sattyre-Library.o

clean:
rm -rf $(BUILD_DIR)

.PHONY: all clean dirs c-abi
