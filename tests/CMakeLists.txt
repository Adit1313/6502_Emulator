include(CTest)
include(Catch)

# ---- LDA ----
add_executable(LDA_Tests tLDA.cpp)
target_link_libraries(LDA_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(LDA_Tests)

# ---- LDX ----
add_executable(LDX_Tests tLDX.cpp)
target_link_libraries(LDX_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(LDX_Tests)

# ---- LDY ----
add_executable(LDY_Tests tLDY.cpp)
target_link_libraries(LDY_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(LDY_Tests)

# ---- TAX, TAY, TXA, TYA ----
add_executable(Transfer_Tests tTransfer.cpp)
target_link_libraries(Transfer_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Transfer_Tests)