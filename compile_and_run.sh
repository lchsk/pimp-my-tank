#!/usr/bin/env bash

set -e

echo "This program requires C++14 support (e.g. g++-6)"

cmake -DCMAKE_CXX_COMPILER=g++-6 .

echo "Starting compilation"

make

echo "Executable should now be ready. Starting application..."

./pimp-my-tank
