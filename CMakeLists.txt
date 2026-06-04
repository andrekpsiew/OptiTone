cmake_minimum_required(VERSION 4.3)
project(OptiTone VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/source.cpp
    src/pipeline.cpp
    src/sink.cpp
)

find_package(SDL2 REQUIRED)

include_directories(/opt/homebrew/include)

target_link_libraries(
    ${PROJECT_NAME}
    PRIVATE
    /opt/homebrew/lib/libportaudio.dylib
    SDL2::SDL2
)