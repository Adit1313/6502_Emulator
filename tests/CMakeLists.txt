include(CTest)
include(Catch)

# ---- LDA, LDX, LDY ----
add_executable(Load_Tests src/tLoad.cpp)
target_include_directories(Load_Tests PRIVATE include)
target_link_libraries(Load_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Load_Tests)

# ---- AND, BIT, EOR, ORA ----
add_executable(Logical_Tests src/tLogical.cpp)
target_include_directories(Logical_Tests PRIVATE include)
target_link_libraries(Logical_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Logical_Tests)

# ---- TSX, TXS, PHA, PHP, PLA, PLP ----
add_executable(Stack_Tests src/tStack.cpp)
target_include_directories(Stack_Tests PRIVATE include)
target_link_libraries(Stack_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Stack_Tests)

# ---- STA, STX, STY ----
add_executable(Store_Tests src/tStore.cpp)
target_include_directories(Store_Tests PRIVATE include)
target_link_libraries(Store_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Store_Tests)

# ---- TAX, TAY, TXA, TYA ----
add_executable(Transfer_Tests src/tTransfer.cpp)
target_include_directories(Transfer_Tests PRIVATE include)
target_link_libraries(Transfer_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Transfer_Tests)

# ---- ADC, SBC, CMP, CPX, CPY ----
add_executable(Arithmetic_Tests src/tArithmetic.cpp)
target_include_directories(Arithmetic_Tests PRIVATE include)
target_link_libraries(Arithmetic_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Arithmetic_Tests)

# ---- INC, INX, INY, DEC, DEX, DEY ----
add_executable(Inc_Dec_Tests src/tIncDec.cpp)
target_include_directories(Inc_Dec_Tests PRIVATE include)
target_link_libraries(Inc_Dec_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Inc_Dec_Tests)

# ---- SEC ----
add_executable(Status_Flag_Tests src/tStatusFlags.cpp)
target_include_directories(Status_Flag_Tests PRIVATE include)
target_link_libraries(Status_Flag_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Status_Flag_Tests)

# ---- ASL, LSR, ROL, ROR ----
add_executable(Shift_Tests src/tShift.cpp)
target_include_directories(Shift_Tests PRIVATE include)
target_link_libraries(Shift_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Shift_Tests)

# ---- BCC, BCS, BEQ, BMI, BNE, BPL, BVC, BVS ----
add_executable(Branch_Tests src/tBranch.cpp)
target_include_directories(Branch_Tests PRIVATE include)
target_link_libraries(Branch_Tests PRIVATE emu_core Catch2::Catch2WithMain)
catch_discover_tests(Branch_Tests)
