cmake_minimum_required(VERSION 3.16)

project(emu6502)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Create executable
add_executable(emu
    emu/emu.cpp
    src/cpu_6502.cpp
    src/bus.cpp
)

# Include directories
target_include_directories(emu PRIVATE
    include
    emu
)