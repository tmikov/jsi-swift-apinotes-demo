#!/bin/bash
# Build the Swift demo against the pristine C++ JSI module.
# Run from the repo root: ./build.sh
set -euo pipefail
cd "$(dirname "$0")"

swiftc -cxx-interoperability-mode=default \
       -I . \
       main.swift jsi.cpp \
       -o app

./app
