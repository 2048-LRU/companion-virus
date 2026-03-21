#!/bin/bash

set -e

sudo apt-get update
sudo apt-get install -y --no-install-recommends \
    build-essential cmake pkg-config \
    libgtk-4-dev libglib2.0-dev libgio-2.0-0 \
    libssl-dev

mkdir -p build && cd build
cmake ..
make
bash ../setup_test_env.sh