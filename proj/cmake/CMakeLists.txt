cmake_minimum_required(VERSION 3.5)
project(generic_eval C CXX)

if(${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR})
    message(FATAL_ERROR "In-source build forbidden.")
    return()
endif()

set(ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../..")
set(MAIN_DIR "${ROOT_DIR}/src/main")

add_subdirectory("${ROOT_DIR}/src/main" "${CMAKE_BINARY_DIR}/main")
