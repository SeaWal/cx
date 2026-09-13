cmake_minimum_required(VERSION 3.20)

project(cx
    VERSION 0.1.0
    LANGUAGES C
)

add_library(cx STATIC 
    src/cx_allocator.c
)

target_include_directories(cx PUBLIC
    $(PROJECT_SOURCE_DIR)/include
)

target_compile_features(cx PUBLIC c_std_11)

if (CMAKE_C_COMPILER_ID MATCHES "clang")
    target_compile_options(cx PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
    )
endif()