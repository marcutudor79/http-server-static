# INCLUDE PROJECT
cmake_minimum_required(VERSION 3.17)
project(http-server C ASM)
set (COMPILE_FLAGS "-o2 -Wpedantic")

add_executable(http-server
    main.c
    utils.c
    lib/file_helper.c
    lib/http_helper.c
)

target_include_directories(http-server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/lib
)