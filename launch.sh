#!/bin/bash

set -e

docker build -f .devcontainer/Dockerfile -t companion-virus:latest .

docker run -it --rm \
    --name companion-virus-test \
    -v "$(pwd)":/workspace \
    -w /workspace \
    companion-virus:latest \
    bash -c "
        mkdir -p build
        cd build
        cmake ..
        make
        bash ../setup_test_env.sh
        cd TPUser
        /bin/bash"