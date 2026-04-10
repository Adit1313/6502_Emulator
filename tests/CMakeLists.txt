add_executable(LDA_Tests tLDA.cpp)

target_link_libraries(LDA_Tests
PRIVATE 
emu_core
Catch2::Catch2WithMain)

# add_test(NAME LDA_Tests COMMAND LDA_Tests)
include(CTest)
include(Catch)

catch_discover_tests(LDA_Tests)