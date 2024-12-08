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

set(STX_TESTS tests/STX/tSTX.cpp)
set(BASE_TEST tests/BaseTest/BaseTest.cpp)
file(GLOB MPU_SRC "src/*.cpp")
list(APPEND STX_TESTS ${MPU_SRC})
list(APPEND STX_TESTS ${BASE_TEST})

add_executable(tSTX ${STX_TESTS})
target_include_directories(tSTX
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/BaseTest
)

set(STY_TESTS tests/STY/tSTY.cpp)
set(BASE_TEST tests/BaseTest/BaseTest.cpp)
file(GLOB MPU_SRC "src/*.cpp")
list(APPEND STY_TESTS ${MPU_SRC})
list(APPEND STY_TESTS ${BASE_TEST})

add_executable(tSTY ${STY_TESTS})
target_include_directories(tSTY
    PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/tests/BaseTest
)