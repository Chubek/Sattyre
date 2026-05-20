#!/usr/bin/env sh
set -eu

MODE="${1:-release}"
BUILD_DIR="build/${MODE}"

case "$MODE" in
  debug) BUILD_TYPE="Debug" ;;
  release) BUILD_TYPE="Release" ;;
  *)
    echo "usage: ./build.sh [debug|release]" >&2
    exit 2
    ;;
esac

cmake -S . -B "${BUILD_DIR}" \
  -DCMAKE_BUILD_TYPE="${BUILD_TYPE}" \
  -DSATTYRE_ENABLE_LUA=OFF \
  -DSATTYRE_ENABLE_LOG4CPLUS=OFF
cmake --build "${BUILD_DIR}"

echo "Build complete: ${BUILD_DIR}"
