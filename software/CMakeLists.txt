# INCLUDE PROJECT
cmake_minimum_required(VERSION 3.17)
set (CMAKE_C_COMPILER "gcc")

project(http-server C)

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

target_compile_options(http-server PRIVATE
    -g
)