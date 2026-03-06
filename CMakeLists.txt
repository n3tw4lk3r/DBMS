cmake_minimum_required(VERSION 3.20)

project(DBMS LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE DBMS_SOURCES
    src/*.cpp
)

add_executable(dbms ${DBMS_SOURCES})

target_include_directories(dbms
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(dbms
    PRIVATE
        -Wall
        -Wextra
        -Wpedantic
)

target_compile_features(dbms PRIVATE cxx_std_20)
