#include "emulator.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>

#pragma region ADC Tests

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

#pragma endregion

#pragma region SBC Tests

TEST_CASE("SBC flag behaviour", "[SBC]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify C flag set (no borrow)")
    {
        // SEC, LDA #0x05, SBC #0x03 => A = 0x02, C=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x05, 0xE9, 0x03});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }

    SECTION("Verify C flag clear (borrow)")
    {
        // SEC, LDA #0x03, SBC #0x05 => A = 0xFE, C=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x03, 0xE9, 0x05});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
    }

    SECTION("Verify Z flag set")
    {
        // SEC, LDA #0x05, SBC #0x05 => A = 0x00, Z=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x05, 0xE9, 0x05});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag clear")
    {
        // SEC, LDA #0x05, SBC #0x03 => A = 0x02, Z=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x05, 0xE9, 0x03});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }

    SECTION("Verify N flag set")
    {
        // SEC, LDA #0x03, SBC #0x05 => A = 0xFE, N=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x03, 0xE9, 0x05});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag clear")
    {
        // SEC, LDA #0x05, SBC #0x03 => A = 0x02, N=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x05, 0xE9, 0x03});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify V flag set (signed overflow)")
    {
        // SEC, LDA #0x80, SBC #0x01 => A = 0x7F, V=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x80, 0xE9, 0x01});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::V) == 1);
    }

    SECTION("Verify V flag clear")
    {
        // SEC, LDA #0x05, SBC #0x03 => A = 0x02, V=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x05, 0xE9, 0x03});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
    }

    /*
    TODO: Add when 
    SECTION("Verify borrow when C=0 initially")
    {
        // CLC, LDA #0x05, SBC #0x03 => A = 0x01
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x18, 0xA9, 0x05, 0xE9, 0x03});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }
    */
}

TEST_CASE("SBC with IMM addressing", "[SBC][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Basic subtraction with SEC")
    {
        // SEC, LDA #0x08, SBC #0x03 => A = 0x05
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x08, 0xE9, 0x03});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x05);
    }

    SECTION("Subtraction without SEC (borrow applied)")
    {
        // LDA #0x08, SBC #0x03 => A = 0x04
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x08, 0xE9, 0x03});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x04);
    }
}

TEST_CASE("SBC with ZP addressing", "[SBC][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Zero page with SEC")
    {
        // mem[0x10]=0x03, SEC, LDA #0x08, SBC $10 => A = 0x05
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x08, 0xE5, 0x10});
        emu.execute(7);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x05);
    }

    SECTION("Zero page without SEC")
    {
        // mem[0x10]=0x03, LDA #0x08, SBC $10 => A = 0x04
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x08, 0xE5, 0x10});
        emu.execute(5);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x04);
    }
}

TEST_CASE("SBC with ZPX addressing", "[SBC][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ZPX with SEC")
    {
        // X=1, mem[0x11]=0x03, SEC, LDA #0x08, SBC $10,X => A = 0x05
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA2, 0x01, 0xA9, 0x08, 0xF5, 0x10});
        emu.execute(8);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x05);
    }

    SECTION("ZPX without SEC")
    {
        // X=1, mem[0x11]=0x03, LDA #0x08, SBC $10,X => A = 0x04
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x08, 0xF5, 0x10});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x04);
    }
}

TEST_CASE("SBC with ABS addressing", "[SBC][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABS with SEC")
    {
        // mem[0x300]=0x03, SEC, LDA #0x08, SBC $0300 => A = 0x05
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA9, 0x08, 0xED, 0x00, 0x03});
        emu.execute(8);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x05);
    }

    SECTION("ABS without SEC")
    {
        // mem[0x300]=0x03, LDA #0x08, SBC $0300 => A = 0x04
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x08, 0xED, 0x00, 0x03});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x04);
    }
}

TEST_CASE("SBC with ABSX addressing", "[SBC][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABSX with SEC")
    {
        // X=1, mem[0x301]=0x03, SEC, LDA #0x08, SBC $0300,X => A = 0x05
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA2, 0x01, 0xA9, 0x08, 0xFD, 0x00, 0x03});
        emu.execute(9);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x05);
    }

    SECTION("ABSX without SEC")
    {
        // X=1, mem[0x301]=0x03, LDA #0x08, SBC $0300,X => A = 0x04
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x08, 0xFD, 0x00, 0x03});
        emu.execute(7);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x04);
    }
}

TEST_CASE("SBC with ABSY addressing", "[SBC][ABSY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABSY with SEC")
    {
        // Y=1, mem[0x301]=0x03, SEC, LDA #0x08, SBC $0300,Y => A = 0x05
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA0, 0x01, 0xA9, 0x08, 0xF9, 0x00, 0x03});
        emu.execute(9);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x05);
    }

    SECTION("ABSY without SEC")
    {
        // Y=1, mem[0x301]=0x03, LDA #0x08, SBC $0300,Y => A = 0x04
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x01, 0xA9, 0x08, 0xF9, 0x00, 0x03});
        emu.execute(7);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x04);
    }
}

TEST_CASE("SBC with IZX addressing", "[SBC][IZX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("IZX with SEC")
    {
        // X=1, ptr -> 0x300, mem[0x300]=0x03
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x00});
        emu.load_bytes_at_address(0x12, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x03});

        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA2, 0x01, 0xA9, 0x08, 0xE1, 0x10});
        emu.execute(10);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x05);
    }

    SECTION("IZX without SEC")
    {
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x00});
        emu.load_bytes_at_address(0x12, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x03});

        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x08, 0xE1, 0x10});
        emu.execute(8);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x04);
    }
}

TEST_CASE("SBC with IZY addressing", "[SBC][IZY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("IZY with SEC")
    {
        // ptr -> 0x300, Y=1 => 0x301, mem=0x03
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x00});
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x03});

        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0xA0, 0x01, 0xA9, 0x08, 0xF1, 0x10});
        emu.execute(9);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x05);
    }

    SECTION("IZY without SEC")
    {
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x00});
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x03});

        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x01, 0xA9, 0x08, 0xF1, 0x10});
        emu.execute(7);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x04);
    }
}

#pragma endregion

#pragma region CMP Tests

TEST_CASE("CMP flag behaviour", "[CMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("A > M (C=1, Z=0, N=0)")
    {
        // LDA #0x08, CMP #0x03
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x08, 0xC9, 0x03});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();

        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("A == M (C=1, Z=1, N=0)")
    {
        // LDA #0x05, CMP #0x05
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x05, 0xC9, 0x05});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();

        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("A < M (C=0, Z=0, N=1)")
    {
        // LDA #0x03, CMP #0x05
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x03, 0xC9, 0x05});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();

        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Negative result without borrow edge case")
    {
        // LDA #0x80, CMP #0x7F => result positive, N=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x80, 0xC9, 0x7F});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();

        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Sign bit propagation")
    {
        // LDA #0x00, CMP #0x80 => result = 0x80, N=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xC9, 0x80});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();

        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }
}

TEST_CASE("CMP with IMM addressing", "[CMP][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Immediate compare greater")
    {
        // LDA #0x0A, CMP #0x02
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x0A, 0xC9, 0x02});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }

    SECTION("Immediate compare equal")
    {
        // LDA #0x0A, CMP #0x0A
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x0A, 0xC9, 0x0A});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Immediate compare less")
    {
        // LDA #0x02, CMP #0x0A
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x02, 0xC9, 0x0A});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
    }
}

TEST_CASE("CMP with ZP addressing", "[CMP][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Zero page compare")
    {
        // mem[0x10]=0x05, LDA #0x08, CMP $10
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x08, 0xC5, 0x10});
        emu.execute(5);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

TEST_CASE("CMP with ZPX addressing", "[CMP][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ZPX compare")
    {
        // X=1, mem[0x11]=0x05, LDA #0x08, CMP $10,X
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x08, 0xD5, 0x10});
        emu.execute(6);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

TEST_CASE("CMP with ABS addressing", "[CMP][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABS compare")
    {
        // mem[0x300]=0x05, LDA #0x08, CMP $0300
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x08, 0xCD, 0x00, 0x03});
        emu.execute(6);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

TEST_CASE("CMP with ABSX addressing", "[CMP][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABSX compare")
    {
        // X=1, mem[0x301]=0x05, LDA #0x08, CMP $0300,X
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x08, 0xDD, 0x00, 0x03});
        emu.execute(7);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

TEST_CASE("CMP with ABSY addressing", "[CMP][ABSY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABSY compare")
    {
        // Y=1, mem[0x301]=0x05, LDA #0x08, CMP $0300,Y
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x01, 0xA9, 0x08, 0xD9, 0x00, 0x03});
        emu.execute(7);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

TEST_CASE("CMP with IZX addressing", "[CMP][IZX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("IZX compare")
    {
        // X=1, ptr -> 0x300, mem[0x300]=0x05
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x00});
        emu.load_bytes_at_address(0x12, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x05});

        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x08, 0xC1, 0x10});
        emu.execute(8);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

TEST_CASE("CMP with IZY addressing", "[CMP][IZY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("IZY compare")
    {
        // ptr -> 0x300, Y=1 => 0x301, mem=0x05
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x00});
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x03});
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x05});

        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x01, 0xA9, 0x08, 0xD1, 0x10});
        emu.execute(7);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

#pragma endregion

#pragma region CPX Tests

TEST_CASE("CPX flag behaviour", "[CPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("X > M (C=1, Z=0, N=0)")
    {
        // LDX #0x08, CPX #0x03
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x08, 0xE0, 0x03});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();

        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("X == M (C=1, Z=1, N=0)")
    {
        // LDX #0x05, CPX #0x05
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x05, 0xE0, 0x05});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();

        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("X < M (C=0, Z=0, N=1)")
    {
        // LDX #0x03, CPX #0x05
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x03, 0xE0, 0x05});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();

        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }
}

TEST_CASE("CPX with IMM addressing", "[CPX][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Immediate compare greater")
    {
        // LDX #0x0A, CPX #0x02
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x0A, 0xE0, 0x02});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }

    SECTION("Immediate compare equal")
    {
        // LDX #0x0A, CPX #0x0A
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x0A, 0xE0, 0x0A});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Immediate compare less")
    {
        // LDX #0x02, CPX #0x0A
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x02, 0xE0, 0x0A});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
    }
}

TEST_CASE("CPX with ZP addressing", "[CPX][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Zero page compare")
    {
        // mem[0x10]=0x05, LDX #0x08, CPX $10
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x08, 0xE4, 0x10});
        emu.execute(5);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

TEST_CASE("CPX with ABS addressing", "[CPX][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Absolute compare")
    {
        // mem[0x300]=0x05, LDX #0x08, CPX $0300
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x08, 0xEC, 0x00, 0x03});
        emu.execute(6);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

TEST_CASE("CPY flag behaviour", "[CPY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Y > M (C=1, Z=0, N=0)")
    {
        // LDY #0x08, CPY #0x03
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x08, 0xC0, 0x03});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();

        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Y == M (C=1, Z=1, N=0)")
    {
        // LDY #0x05, CPY #0x05
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x05, 0xC0, 0x05});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();

        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Y < M (C=0, Z=0, N=1)")
    {
        // LDY #0x03, CPY #0x05
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x03, 0xC0, 0x05});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();

        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }
}

TEST_CASE("CPY with IMM addressing", "[CPY][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Immediate compare greater")
    {
        // LDY #0x0A, CPY #0x02
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x0A, 0xC0, 0x02});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }

    SECTION("Immediate compare equal")
    {
        // LDY #0x0A, CPY #0x0A
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x0A, 0xC0, 0x0A});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Immediate compare less")
    {
        // LDY #0x02, CPY #0x0A
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x02, 0xC0, 0x0A});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
    }
}

TEST_CASE("CPY with ZP addressing", "[CPY][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Zero page compare")
    {
        // mem[0x10]=0x05, LDY #0x08, CPY $10
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x08, 0xC4, 0x10});
        emu.execute(5);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

TEST_CASE("CPY with ABS addressing", "[CPY][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Absolute compare")
    {
        // mem[0x300]=0x05, LDY #0x08, CPY $0300
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x05});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x08, 0xCC, 0x00, 0x03});
        emu.execute(6);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

#pragma endregion