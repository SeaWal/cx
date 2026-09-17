cmake_minimum_required(VERSION 3.20)

project(cx
    VERSION 0.1.0
    LANGUAGES C
)

add_library(cx STATIC 
    src/cx_allocator.c
)

target_include_directories(cx PUBLIC
    ${PROJECT_SOURCE_DIR}/include
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

# tests (TODO: fix when using proper framework)
enable_testing()

function(cx_add_test TEST_NAME SOURCE_FILE)
    add_executable(${TEST_NAME} ${SOURCE_FILE})

    target_link_libraries(${TEST_NAME} PRIVATE cx)

    target_compile_options(${TEST_NAME} PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wshadow
    )

    add_test(
        NAME ${TEST_NAME}
        COMMAND ${TEST_NAME}
    )
endfunction()

cx_add_test(test_general_allocator tests/test_general_alloc.c)
