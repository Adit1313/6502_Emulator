cmake_minimum_required(VERSION 3.20)

project(mpu6502_emulator_tests)

set(CMAKE_CXX_STANDARD 20)

set(LD_TESTS tests/LoadTests/tLoadAXYTests.cpp)
set(BASE_TEST tests/BaseTest/BaseTest.cpp)
file(GLOB MPU_SRC "src/*.cpp")
list(APPEND LD_TESTS ${MPU_SRC})
list(APPEND LD_TESTS ${BASE_TEST})

add_executable(tLoadTests ${LD_TESTS})
target_include_directories(tLoadTests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/BaseTest
)

set(ST_TESTS tests/StoreTests/tStoreAXYTests.cpp)
set(BASE_TEST tests/BaseTest/BaseTest.cpp)
file(GLOB MPU_SRC "src/*.cpp")
list(APPEND ST_TESTS ${MPU_SRC})
list(APPEND ST_TESTS ${BASE_TEST})

add_executable(tStoreAXYTests ${ST_TESTS})
target_include_directories(tStoreAXYTests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/BaseTest
)

set(JC_TESTS tests/JumpTests/tJumpTests.cpp)
set(BASE_TEST tests/BaseTest/BaseTest.cpp)
file(GLOB MPU_SRC "src/*.cpp")
list(APPEND JC_TESTS ${MPU_SRC})
list(APPEND JC_TESTS ${BASE_TEST})

add_executable(tJumpTests ${JC_TESTS})
target_include_directories(tJumpTests
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/BaseTest
)
