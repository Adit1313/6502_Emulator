#include "emulator.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>

TEST_CASE("LDX flag behaviour", "[LDX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify N flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag clear")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F, 0xA2, 0x2});
        emu.execute(4); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify Z flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x0});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag clear")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x0, 0xA2, 0x1});
        emu.execute(4); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
}

TEST_CASE("LDX with IMM addressing", "[LDX][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(state.X == 0x8F);
    }
}

TEST_CASE("LDX with ZP addressing", "[LDX][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA6, 0x20});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x20, std::vector<u8> {0xA5});
        emu.execute(3); 
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(state.X == 0xA5);
    }
}

TEST_CASE("LDX with ZPY addressing", "[LDX][ZPY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x1, 0xB6, 0x20});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x21, std::vector<u8> {0xA5});
        emu.execute(5);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(state.X == 0xA5);
    }
}

TEST_CASE("LDX with ABS addressing", "[LDX][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xAE, 0x12, 0x34});
    emu.load_bytes_at_address(0x1234, std::vector<u8> {0x69});
    /*
    LDX #1
    LDX $20,X
    */
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify accumulator value")
    {
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(state.X == 0x69);
    }
}

TEST_CASE("LDX with ABSY addressing", "[LDX][ABSY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x5, 0xBE, 0x12, 0x34});
    emu.load_bytes_at_address(0x1239, std::vector<u8> {0x69});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify accumulator value")
    {
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(state.X == 0x69);
    }
}