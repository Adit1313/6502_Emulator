cmake_minimum_required(VERSION 3.20)

project(mpu6502_emulator_tests)

set(CMAKE_CXX_STANDARD 20)

set(LDA_TESTS tests/LDA/tLDA.cpp)
set(BASE_TEST tests/BaseTest/BaseTest.cpp)
file(GLOB MPU_SRC "src/*.cpp")
list(APPEND LDA_TESTS ${MPU_SRC})
list(APPEND LDA_TESTS ${BASE_TEST})

add_executable(tLDA ${LDA_TESTS})
target_include_directories(tLDA
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/BaseTest
)

set(LDX_TESTS tests/LDX/tLDX.cpp)
set(BASE_TEST tests/BaseTest/BaseTest.cpp)
file(GLOB MPU_SRC "src/*.cpp")
list(APPEND LDX_TESTS ${MPU_SRC})
list(APPEND LDX_TESTS ${BASE_TEST})

add_executable(tLDX ${LDX_TESTS})
target_include_directories(tLDX
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/BaseTest
)

set(LDY_TESTS tests/LDY/tLDY.cpp)
set(BASE_TEST tests/BaseTest/BaseTest.cpp)
file(GLOB MPU_SRC "src/*.cpp")
list(APPEND LDY_TESTS ${MPU_SRC})
list(APPEND LDY_TESTS ${BASE_TEST})

add_executable(tLDY ${LDY_TESTS})
target_include_directories(tLDY
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/BaseTest
)

set(STA_TESTS tests/STA/tSTA.cpp)
set(BASE_TEST tests/BaseTest/BaseTest.cpp)
file(GLOB MPU_SRC "src/*.cpp")
list(APPEND STA_TESTS ${MPU_SRC})
list(APPEND STA_TESTS ${BASE_TEST})

add_executable(tSTA ${STA_TESTS})
target_include_directories(tSTA
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/BaseTest
)
