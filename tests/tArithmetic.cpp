#include "emulator.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>

TEST_CASE("ADC flag behaviour", "[ADC]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify C flag set (overflow past 0xFF)")
    {
        // LDA #0xFF, ADC #0x01 => A = 0x00, C=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x69, 0x01});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }

    SECTION("Verify C flag clear")
    {
        // LDA #0x01, ADC #0x01 => A = 0x02, C=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x01, 0x69, 0x01});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
    }

    SECTION("Verify Z flag set")
    {
        // LDA #0xFF, ADC #0x01 => A = 0x00, Z=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x69, 0x01});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag clear")
    {
        // LDA #0x01, ADC #0x01 => A = 0x02, Z=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x01, 0x69, 0x01});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }

    SECTION("Verify N flag set")
    {
        // LDA #0x40, ADC #0x40 => A = 0x80, N=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x40, 0x69, 0x40});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag clear")
    {
        // LDA #0x01, ADC #0x01 => A = 0x02, N=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x01, 0x69, 0x01});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify V flag set (signed overflow)")
    {
        // LDA #0x7F, ADC #0x01 => A = 0x80, V=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x7F, 0x69, 0x01});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::V) == 1);
    }

    SECTION("Verify V flag clear")
    {
        // LDA #0x01, ADC #0x01 => A = 0x02, V=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x01, 0x69, 0x01});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
    }

    /*
    TODO: Enable whewn SEC added
    SECTION("Verify carry-in is used")
    {
        // SEC, LDA #0x01, ADC #0x01 => A = 0x03
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x01, 0x69, 0x01});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x03);
    }
    */
}

TEST_CASE("ADC with IMM addressing", "[ADC][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Basic addition")
    {
        // LDA #0x10, ADC #0x05 => A = 0x15
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x10, 0x69, 0x05});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x15);
    }

    SECTION("Addition with carry-out")
    {
        // LDA #0xFF, ADC #0x02 => A = 0x01, C=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x69, 0x02});
        emu.execute(4); 
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }
}

TEST_CASE("ADC with ZP addressing", "[ADC][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Zero page addition")
    {
        // mem[0x10] = 0x05, LDA #0x03, ADC $10 => A = 0x08
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x03, 0x65, 0x10});
        emu.execute(5);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x08);
    }
}

TEST_CASE("ADC with ZPX addressing", "[ADC][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Zero page X indexed")
    {
        // X=1, mem[0x11]=0x05, LDA #0x03, ADC $10,X => A = 0x08
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x03, 0x75, 0x10});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x08);
    }
}

TEST_CASE("ADC with ABS addressing", "[ADC][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Absolute addressing")
    {
        // mem[0x300]=0x05, LDA #0x03, ADC $0300 => A = 0x08
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x03, 0x6D, 0x0, 0x3});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x08);
    }
}

TEST_CASE("ADC with ABSX addressing", "[ADC][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Absolute X indexed")
    {
        // X=1, mem[0x301]=0x05, LDA #0x03, ADC $0300,X => A = 0x08
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x03, 0x7D, 0x0, 0x3});
        emu.execute(7);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x08);
    }
}

TEST_CASE("ADC with ABSY addressing", "[ADC][ABSY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Absolute Y indexed")
    {
        // Y=1, mem[0x301]=0x05, LDA #0x03, ADC $0300,Y => A = 0x08
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x01, 0xA9, 0x03, 0x79, 0x0, 0x3});
        emu.execute(7);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x08);
    }
}

TEST_CASE("ADC with IZX addressing", "[ADC][IZX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Indexed indirect")
    {
        // X=1, ptr at (0x10+X)=0x11 -> points to 0x300, mem[0x300]=0x05
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x0});
        emu.load_bytes_at_address(0x12, std::vector<u8> {0x3});
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x05});

        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x03, 0x61, 0x10});
        emu.execute(8);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x08);
    }
}

TEST_CASE("ADC with IZY addressing", "[ADC][IZY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Indirect indexed")
    {
        // ptr at 0x10 -> 0x300, Y=1 => 0x301, mem[0x301]=0x05
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x00});
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x05});

        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x01, 0xA9, 0x03, 0x71, 0x10});
        emu.execute(7);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x08);
    }
}