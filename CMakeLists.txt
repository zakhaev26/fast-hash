cmake_minimum_required(VERSION 3.10)
project(FastHash)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.11.2
)
FetchContent_MakeAvailable(json)

include_directories(include)

add_executable(FastHash
    src/main.cpp
    src/core/command.cpp
    src/core/fast-hash.cpp
    src/core/ttl_manager.cpp
    src/core/parser.cpp
)

target_link_libraries(FastHash PRIVATE nlohmann_json::nlohmann_json)
