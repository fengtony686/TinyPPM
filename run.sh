#!/usr/bin/env bash

# If project not ready, generate cmake file.
if [[ ! -d build ]]; then
    echo "good"
else
    rm -rf build
fi
mkdir -p build
# shellcheck disable=SC2164
cd build
cmake ..
make -j
# shellcheck disable=SC2103
cd ..
./build/PPM