#!/bin/bash

set -e

BUILD_DIR="build"

case "${1:-build}" in
    build)
        cmake -S . -B "$BUILD_DIR"
        cmake --build "$BUILD_DIR"
        ;;

    clean)
        rm -rf "$BUILD_DIR"
        ;;

    rebuild)
        rm -rf "$BUILD_DIR"
        cmake -S . -B "$BUILD_DIR"
        cmake --build "$BUILD_DIR"
        ;;

    test)
        cmake -S . -B "$BUILD_DIR"
        cmake --build "$BUILD_DIR"
        ctest --test-dir "$BUILD_DIR" --output-on-failure
        ;;

    *)
        echo "Usage: $0 [build|clean|rebuild]"
        exit 1
        ;;
esac