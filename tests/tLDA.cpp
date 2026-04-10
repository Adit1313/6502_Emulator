#include "emulator.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>

TEST_CASE("LDA with IMM addressing", "[LDA][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(state.A == 0x8F);
    }

    SECTION("Verify N flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag clear")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F, 0xA9, 0x2});
        emu.execute(4); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify Z flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x0});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag clear")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x0, 0xA9, 0x1});
        emu.execute(4); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
}

TEST_CASE("LDA with ZP addressing", "[LDA][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA5, 0x20});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x20, std::vector<u8> {0xA5});
        emu.execute(3); 
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(state.A == 0xA5);
    }

    SECTION("Verify N flag set")
    {
        emu.load_bytes_at_address(0x20, std::vector<u8> {0x8F});
        emu.execute(3); 
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag clear")
    {
        emu.load_bytes_at_address(0x20, std::vector<u8> {0x1});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F, 0xA5, 0x2});
        emu.execute(5); 
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify Z flag set")
    {
        emu.load_bytes_at_address(0x20, std::vector<u8> {0x0});
        emu.execute(3); 
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag clear")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x0, 0xA5, 0x20});
        emu.load_bytes_at_address(0x20, std::vector<u8> {0x1});
        emu.execute(5); 
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
}

TEST_CASE("LDA with ZPX addressing", "[LDA][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x1, 0xB5, 0x20});
    /*
    LDX #1
    LDA $20,X
    */
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x21, std::vector<u8> {0xA5});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_State();
        REQUIRE(state.A == 0xA5);
    }

    // SECTION("Verify N flag set")
    // {
    //     emu.load_bytes_at_address(0x21, std::vector<u8> {0x8F});
    //     emu.execute(3);
    //     auto cpu = emu.get_CPU_obj();
    //     auto state = cpu.get_CPU_State();
    //     REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    // }

    // SECTION("Verify N flag clear")
    // {
    //     emu.load_bytes_at_address(0x20, std::vector<u8> {0x1});
    //     emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F, 0xA5, 0x2});
    //     emu.execute(5); 
    //     auto cpu = emu.get_CPU_obj();
    //     auto state = cpu.get_CPU_State();
    //     REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    // }

    // SECTION("Verify Z flag set")
    // {
    //     emu.load_bytes_at_address(0x20, std::vector<u8> {0x0});
    //     emu.execute(3); 
    //     auto cpu = emu.get_CPU_obj();
    //     auto state = cpu.get_CPU_State();
    //     REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    // }

    // SECTION("Verify Z flag clear")
    // {
    //     emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x0, 0xA5, 0x20});
    //     emu.load_bytes_at_address(0x20, std::vector<u8> {0x1});
    //     emu.execute(5); 
    //     auto cpu = emu.get_CPU_obj();
    //     auto state = cpu.get_CPU_State();
    //     REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    // }
}
