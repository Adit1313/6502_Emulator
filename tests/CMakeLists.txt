add_executable(LDA_Tests tLDA.cpp)

target_link_libraries(LDA_Tests emu_core)

add_test(NAME LDA_Tests COMMAND LDA_Tests)