cmake_minimum_required(VERSION 3.16)
project(emu6502)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v3.5.0
)

FetchContent_MakeAvailable(Catch2)

# Core library
add_library(emu_core
    src/cpu_6502.cpp
    src/bus.cpp
    src/emulator.cpp
)

target_include_directories(emu_core PUBLIC include)

# Main executable
add_executable(emu emu/main.cpp)
target_link_libraries(emu emu_core)

# Enable testing
enable_testing()

# Add tests
add_subdirectory(tests)