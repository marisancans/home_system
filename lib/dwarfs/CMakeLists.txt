cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(dwarfs)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SOURCES
    "alias.h"
    "alias.cpp"
)

add_library(${PROJECT_NAME} ${SOURCES})
target_include_directories (${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

