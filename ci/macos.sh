#!/bin/bash
set -euxo pipefail

if ! command -v cmake &> /dev/null; then
    echo "Error: CMake not found. Install with: brew install cmake"
    exit 1
fi

if [[ -z ${BABRAT_ROOT:-} ]]; then
  BABRAT_ROOT="$(realpath $(dirname ${0})/..)"
fi

JOBS=$(sysctl -n hw.logicalcpu)

for cmake_off_on in OFF ON; do
  BUILD_DIR=$(mktemp -d build_dir.XXXXXXXX)
  cd ${BUILD_DIR}
  time cmake ${BABRAT_ROOT} \
    -DCMAKE_CXX_STANDARD=20 \
    -DBUILD_TESTING=ON
   time make -j"${JOBS}"
   time ctest -j"${JOBS}" --output-on-failure
done