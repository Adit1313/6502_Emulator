#include "emulator.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>

// ============================================================
// AND Tests
// ============================================================

TEST_CASE("AND flag behaviour", "[AND]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify N flag set")
    {
        // LDA #0xFF, AND #0x80 => A = 0x80, N=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x29, 0x80});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }
    SECTION("Verify N flag clear")
    {
        // LDA #0xFF, AND #0x7F => A = 0x7F, N=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x29, 0x7F});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }
    SECTION("Verify Z flag set")
    {
        // LDA #0xAA, AND #0x55 => A = 0x00, Z=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x29, 0x55});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }
    SECTION("Verify Z flag clear")
    {
        // LDA #0xFF, AND #0x01 => A = 0x01, Z=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x29, 0x01});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
}

TEST_CASE("AND with IMM addressing", "[AND][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("AND immediate basic mask")
    {
        // LDA #0xFF, AND #0x0F => A = 0x0F
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x29, 0x0F});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x0F);
    }
    SECTION("AND immediate zeroes accumulator")
    {
        // LDA #0xAA, AND #0x55 => A = 0x00
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x29, 0x55});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x00);
    }
}

TEST_CASE("AND with ZP addressing", "[AND][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("AND zero page result")
    {
        // Store 0x0F at ZP address 0x10, LDA #0xFF, AND $10 => A = 0x0F
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x0F});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x25, 0x10});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x0F);
    }
}

TEST_CASE("AND with ZPX addressing", "[AND][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("AND zero page X result")
    {
        // Store 0x3C at ZP 0x15, LDA #0xFF, LDX #0x05, AND $10,X => A = 0x3C
        emu.load_bytes_at_address(0x15, std::vector<u8> {0x3C});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0xA2, 0x05, 0x35, 0x10});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x3C);
    }
}

TEST_CASE("AND with ABS addressing", "[AND][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("AND absolute result")
    {
        // Store 0xF0 at 0x0300, LDA #0xFF, AND $0300 => A = 0xF0
        emu.load_bytes_at_address(0x0300, std::vector<u8> {0xF0});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x2D, 0x0, 0x3});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0xF0);
    }
}

TEST_CASE("AND with ABSX addressing", "[AND][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("AND absolute X result")
    {
        // Store 0x55 at 0x0305, LDA #0xFF, LDX #0x05, AND $0300,X => A = 0x55
        emu.load_bytes_at_address(0x0305, std::vector<u8> {0x55});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0xA2, 0x05, 0x3D, 0x0, 0x3});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x55);
    }
}

TEST_CASE("AND with ABSY addressing", "[AND][ABSY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("AND absolute Y result")
    {
        // Store 0x55 at 0x0305, LDA #0xFF, LDY #0x05, AND $0300,Y => A = 0x55
        emu.load_bytes_at_address(0x0305, std::vector<u8> {0x55});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0xA0, 0x05, 0x39, 0x0, 0x3});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x55);
    }
}

TEST_CASE("AND with IZX addressing", "[AND][IZX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("AND indirect X result")
    {
        // ZP 0x15/0x16 => pointer to 0x0400, store 0x0F at 0x0400
        // LDA #0xFF, LDX #0x05, AND ($10,X) => A = 0x0F
        emu.load_bytes_at_address(0x15, std::vector<u8> {0x0, 0x4});
        emu.load_bytes_at_address(0x0400, std::vector<u8> {0x0F});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0xA2, 0x05, 0x21, 0x10});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x0F);
    }
}

TEST_CASE("AND with IZY addressing", "[AND][IZY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("AND indirect Y result")
    {
        // ZP 0x10/0x11 => base pointer 0x0400, LDY #0x05, => effective 0x0405
        // Store 0x3C at 0x0405, LDA #0xFF, AND ($10),Y => A = 0x3C
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x0, 0x4});
        emu.load_bytes_at_address(0x0405, std::vector<u8> {0x3C});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0xA0, 0x05, 0x31, 0x10});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x3C);
    }
}

// ============================================================
// EOR Tests
// ============================================================

TEST_CASE("EOR flag behaviour", "[EOR]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify N flag set")
    {
        // LDA #0x0F, EOR #0xFF => A = 0xF0, N=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x0F, 0x49, 0xFF});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }
    SECTION("Verify N flag clear")
    {
        // LDA #0xFF, EOR #0xFF => A = 0x00 (N=0, Z=1)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x49, 0xFF});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }
    SECTION("Verify Z flag set")
    {
        // LDA #0xAA, EOR #0xAA => A = 0x00, Z=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x49, 0xAA});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }
    SECTION("Verify Z flag clear")
    {
        // LDA #0xAA, EOR #0x55 => A = 0xFF, Z=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x49, 0x55});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
}

TEST_CASE("EOR with IMM addressing", "[EOR][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("EOR immediate flips bits")
    {
        // LDA #0xAA, EOR #0xFF => A = 0x55
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x49, 0xFF});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x55);
    }
    SECTION("EOR immediate same value zeroes accumulator")
    {
        // LDA #0xBE, EOR #0xBE => A = 0x00
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xBE, 0x49, 0xBE});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x00);
    }
}

TEST_CASE("EOR with ZP addressing", "[EOR][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("EOR zero page result")
    {
        // Store 0xFF at ZP 0x10, LDA #0xAA, EOR $10 => A = 0x55
        emu.load_bytes_at_address(0x10, std::vector<u8> {0xFF});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x45, 0x10});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x55);
    }
}

TEST_CASE("EOR with ZPX addressing", "[EOR][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("EOR zero page X result")
    {
        // Store 0xFF at ZP 0x15, LDA #0xAA, LDX #0x05, EOR $10,X => A = 0x55
        emu.load_bytes_at_address(0x15, std::vector<u8> {0xFF});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0xA2, 0x05, 0x55, 0x10});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x55);
    }
}

TEST_CASE("EOR with ABS addressing", "[EOR][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("EOR absolute result")
    {
        // Store 0xFF at 0x0300, LDA #0xAA, EOR $0300 => A = 0x55
        emu.load_bytes_at_address(0x0300, std::vector<u8> {0xFF});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x4D, 0x0, 0x3});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x55);
    }
}

TEST_CASE("EOR with ABSX addressing", "[EOR][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("EOR absolute X result")
    {
        // Store 0xFF at 0x0305, LDA #0xAA, LDX #0x05, EOR $0300,X => A = 0x55
        emu.load_bytes_at_address(0x0305, std::vector<u8> {0xFF});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0xA2, 0x05, 0x5D, 0x0, 0x3});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x55);
    }
}

TEST_CASE("EOR with ABSY addressing", "[EOR][ABSY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("EOR absolute Y result")
    {
        // Store 0xFF at 0x0305, LDA #0xAA, LDY #0x05, EOR $0300,Y => A = 0x55
        emu.load_bytes_at_address(0x0305, std::vector<u8> {0xFF});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0xA0, 0x05, 0x59, 0x0, 0x3});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x55);
    }
}

TEST_CASE("EOR with IZX addressing", "[EOR][IZX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("EOR indirect X result")
    {
        // ZP 0x15/0x16 => pointer to 0x0400, store 0xFF at 0x0400
        // LDA #0xAA, LDX #0x05, EOR ($10,X) => A = 0x55
        emu.load_bytes_at_address(0x15, std::vector<u8> {0x0, 0x4});
        emu.load_bytes_at_address(0x0400, std::vector<u8> {0xFF});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0xA2, 0x05, 0x41, 0x10});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x55);
    }
}

TEST_CASE("EOR with IZY addressing", "[EOR][IZY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("EOR indirect Y result")
    {
        // ZP 0x10/0x11 => base pointer 0x0400, LDY #0x05 => effective 0x0405
        // Store 0xFF at 0x0405, LDA #0xAA, EOR ($10),Y => A = 0x55
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x0, 0x4});
        emu.load_bytes_at_address(0x0405, std::vector<u8> {0xFF});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0xA0, 0x05, 0x51, 0x10});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x55);
    }
}

// ============================================================
// ORA Tests
// ============================================================

TEST_CASE("ORA flag behaviour", "[ORA]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify N flag set")
    {
        // LDA #0x00, ORA #0x80 => A = 0x80, N=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0x09, 0x80});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }
    SECTION("Verify N flag clear")
    {
        // LDA #0x00, ORA #0x01 => A = 0x01, N=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0x09, 0x01});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }
    SECTION("Verify Z flag set")
    {
        // LDA #0x00, ORA #0x00 => A = 0x00, Z=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0x09, 0x00});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }
    SECTION("Verify Z flag clear")
    {
        // LDA #0x00, ORA #0x01 => A = 0x01, Z=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0x09, 0x01});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
}

TEST_CASE("ORA with IMM addressing", "[ORA][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ORA immediate sets bits")
    {
        // LDA #0xAA, ORA #0x55 => A = 0xFF
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x09, 0x55});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0xFF);
    }
    SECTION("ORA immediate with zero operand is identity")
    {
        // LDA #0x3C, ORA #0x00 => A = 0x3C
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x3C, 0x09, 0x00});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x3C);
    }
}

TEST_CASE("ORA with ZP addressing", "[ORA][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ORA zero page result")
    {
        // Store 0x55 at ZP 0x10, LDA #0xAA, ORA $10 => A = 0xFF
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x55});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x05, 0x10});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0xFF);
    }
}

TEST_CASE("ORA with ZPX addressing", "[ORA][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ORA zero page X result")
    {
        // Store 0x55 at ZP 0x15, LDA #0xAA, LDX #0x05, ORA $10,X => A = 0xFF
        emu.load_bytes_at_address(0x15, std::vector<u8> {0x55});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0xA2, 0x05, 0x15, 0x10});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0xFF);
    }
}

TEST_CASE("ORA with ABS addressing", "[ORA][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ORA absolute result")
    {
        // Store 0x55 at 0x0300, LDA #0xAA, ORA $0300 => A = 0xFF
        emu.load_bytes_at_address(0x0300, std::vector<u8> {0x55});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x0D, 0x0, 0x3});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0xFF);
    }
}

TEST_CASE("ORA with ABSX addressing", "[ORA][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ORA absolute X result")
    {
        // Store 0x55 at 0x0305, LDA #0xAA, LDX #0x05, ORA $0300,X => A = 0xFF
        emu.load_bytes_at_address(0x0305, std::vector<u8> {0x55});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0xA2, 0x05, 0x1D, 0x0, 0x3});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0xFF);
    }
}

TEST_CASE("ORA with ABSY addressing", "[ORA][ABSY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ORA absolute Y result")
    {
        // Store 0x55 at 0x0305, LDA #0xAA, LDY #0x05, ORA $0300,Y => A = 0xFF
        emu.load_bytes_at_address(0x0305, std::vector<u8> {0x55});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0xA0, 0x05, 0x19, 0x0, 0x3});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0xFF);
    }
}

TEST_CASE("ORA with IZX addressing", "[ORA][IZX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ORA indirect X result")
    {
        // ZP 0x15/0x16 => pointer to 0x0400, store 0x55 at 0x0400
        // LDA #0xAA, LDX #0x05, ORA ($10,X) => A = 0xFF
        emu.load_bytes_at_address(0x15, std::vector<u8> {0x0, 0x4});
        emu.load_bytes_at_address(0x0400, std::vector<u8> {0x55});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0xA2, 0x05, 0x01, 0x10});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0xFF);
    }
}

TEST_CASE("ORA with IZY addressing", "[ORA][IZY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ORA indirect Y result")
    {
        // ZP 0x10/0x11 => base pointer 0x0400, LDY #0x05 => effective 0x0405
        // Store 0x55 at 0x0405, LDA #0xAA, ORA ($10),Y => A = 0xFF
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x0, 0x4});
        emu.load_bytes_at_address(0x0405, std::vector<u8> {0x55});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0xA0, 0x05, 0x11, 0x10});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0xFF);
    }
}

// ============================================================
// BIT Tests
// ============================================================

TEST_CASE("BIT flag behaviour", "[BIT]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify Z flag set when A AND mem == 0")
    {
        // Store 0x55 at ZP 0x10, LDA #0xAA, BIT $10 => A&mem=0x00, Z=1
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x0});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x24, 0x10});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }
    SECTION("Verify Z flag clear when A AND mem != 0")
    {
        // Store 0xFF at ZP 0x10, LDA #0xAA, BIT $10 => A&mem=0xAA, Z=0
        emu.load_bytes_at_address(0x10, std::vector<u8> {0xFF});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x24, 0x10});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
    SECTION("Verify N flag set from bit 7 of memory")
    {
        // Store 0x80 at ZP 0x10, LDA #0xFF, BIT $10 => N = mem[7] = 1
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x80});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x24, 0x10});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }
    SECTION("Verify N flag clear from bit 7 of memory")
    {
        // Store 0x7F at ZP 0x10, LDA #0xFF, BIT $10 => N = mem[7] = 0
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x7F});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x24, 0x10});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }
    SECTION("Verify V flag set from bit 6 of memory")
    {
        // Store 0x40 at ZP 0x10, LDA #0xFF, BIT $10 => V = mem[6] = 1
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x40});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x24, 0x10});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::V) == 1);
    }
    SECTION("Verify V flag clear from bit 6 of memory")
    {
        // Store 0x3F at ZP 0x10, LDA #0xFF, BIT $10 => V = mem[6] = 0
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x3F});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x24, 0x10});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
    }
    SECTION("Verify A is unmodified after BIT")
    {
        // BIT must not change the accumulator
        emu.load_bytes_at_address(0x10, std::vector<u8> {0xFF});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x24, 0x10});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0xAA);
    }
}

TEST_CASE("BIT with ZP addressing", "[BIT][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("BIT zero page N and V set, Z clear")
    {
        // Store 0xC0 (bits 7&6 set) at ZP 0x20, LDA #0xFF, BIT $20
        // N=1, V=1, Z=0
        emu.load_bytes_at_address(0x20, std::vector<u8> {0xC0});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x24, 0x20});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::V) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
    SECTION("BIT zero page all flags clear")
    {
        // Store 0x00 at ZP 0x20, LDA #0x00, BIT $20
        // N=0, V=0, Z=1
        emu.load_bytes_at_address(0x20, std::vector<u8> {0x00});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0x24, 0x20});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }
}

TEST_CASE("BIT with ABS addressing", "[BIT][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("BIT absolute N and V set, Z clear")
    {
        // Store 0xC0 at 0x0300, LDA #0xFF, BIT $0300
        // N=1, V=1, Z=0
        emu.load_bytes_at_address(0x0300, std::vector<u8> {0xC0});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xFF, 0x2C, 0x0, 0x3});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::V) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
    SECTION("BIT absolute Z set when masked result is zero")
    {
        // Store 0x55 at 0x0300, LDA #0xAA, BIT $0300 => A&mem=0x00, Z=1
        emu.load_bytes_at_address(0x0300, std::vector<u8> {0x55});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xAA, 0x2C, 0x0, 0x3});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }
    SECTION("BIT absolute A unmodified")
    {
        // Store 0xFF at 0x0300, LDA #0x3C, BIT $0300 => A still 0x3C
        emu.load_bytes_at_address(0x0300, std::vector<u8> {0xFF});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x3C, 0x2C, 0x0, 0x3});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x3C);
    }
}