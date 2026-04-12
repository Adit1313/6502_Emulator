include(CTest)
include(Catch)

# ---- LDA, LDX, LDY ----
add_executable(Load_Tests tLoad.cpp)
target_link_libraries(Load_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Load_Tests)

# ---- AND, BIT, EOR, ORA ----
add_executable(Logical_Tests tLogical.cpp)
target_link_libraries(Logical_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Logical_Tests)

# ---- TSX, TXS, PHA, PHP, PLA, PLP ----
add_executable(Stack_Tests tStack.cpp)
target_link_libraries(Stack_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Stack_Tests)

# ---- STA, STX, STY ----
add_executable(Store_Tests tStore.cpp)
target_link_libraries(Store_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Store_Tests)

# ---- TAX, TAY, TXA, TYA ----
add_executable(Transfer_Tests tTransfer.cpp)
target_link_libraries(Transfer_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Transfer_Tests)

# ---- ADC, SBC, CMP, CPX, CPY ----
add_executable(Arithmetic_Tests tArithmetic.cpp)
target_link_libraries(Arithmetic_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Arithmetic_Tests)