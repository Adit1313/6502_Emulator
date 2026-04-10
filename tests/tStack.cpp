#include "emulator.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>

TEST_CASE("TSX tests","[TSX][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xBA});
    /*
    Test does the following:
    1. Load the accumulator
    2. Push it onto the stack
    3. Verify that value is stack memory
    4. Load a negative value to Y
    5. Push flags to stack
    6. Verify that value is in stack memory
    7. Pull 
    */
    emu.reset(emu.RST_CPU);
    emu.execute(2);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    REQUIRE(state.X == 0xFD);
}

TEST_CASE("TXS tests","[TXS][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0xFE, 0x9A});
    emu.reset(emu.RST_CPU);
    emu.execute(4);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    REQUIRE(state.SP == 0xFE);
}

TEST_CASE("PHA tests","[PHA][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x23, 0x48, 0xAC, 0x01, 0xFD});
    emu.reset(emu.RST_CPU);
    emu.execute(8);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    
    REQUIRE(state.SP == 0xFC);  // Check if SP decremented
    REQUIRE(state.Y == 0x23);   // Check if value in stack is correct
}

TEST_CASE("PHP tests","[PHP][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F, 0x08, 0xAC, 0x01, 0xFD});
    emu.reset(emu.RST_CPU);
    emu.execute(8);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    
    REQUIRE(state.SP == 0xFC);  // Check if SP decremented
    REQUIRE(state.Y == 0x40);   // Check if value in stack is correct
}

TEST_CASE("PLA tests","[PLA][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x23, 0x48, 0xA9, 0x21, 0x48, 0x68, 0x68});
    emu.reset(emu.RST_CPU);
    emu.execute(18);

    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    REQUIRE(state.SP == 0xFD);  // Check if SP back at start
    REQUIRE(state.A == 0x23);   // Check if value in stack is correct
}

TEST_CASE("PLP tests","[PLP][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F, 0x08, 0xA2, 0x0, 0x08, 0xA2, 0x1, 0x28, 0x28});
    emu.reset(emu.RST_CPU);
    emu.execute(16);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    emu.execute(4);
    cpu = emu.get_CPU_obj();
    state = cpu.get_CPU_state();
    REQUIRE(state.SP == 0xFD);
    REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
}