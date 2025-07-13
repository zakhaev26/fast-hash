cmake_minimum_required(VERSION 3.10)
project(FastHash)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(FastHash
    src/main.cpp
    src/core/command.cpp
    src/core/fast-hash.cpp
)
