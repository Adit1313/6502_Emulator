#include "emulator.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>

#pragma region ASL Tests

// ==================== ASL ====================

TEST_CASE("ASL flag behaviour", "[ASL]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Sets carry flag when bit 7 was set")
    {
        // A=0x80 -> ASL -> 0x00, C=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x80,
            0x0A
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x00);
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Clears carry flag when bit 7 was clear")
    {
        // A=0x40 -> ASL -> 0x80, C=0, N=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x40,
            0x0A
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x80);
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Normal shift clears Z and N")
    {
        // A=0x01 -> ASL -> 0x02
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x01,
            0x0A
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }
}

TEST_CASE("ASL with ZP addressing", "[ASL][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Basic shift")
    {
        // mem[0x10]=0x01, ASL $10 => 0x02
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0x06, 0x10,
            0xA5, 0x10
        });
        emu.execute(8);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }
}

TEST_CASE("ASL with ZPX addressing", "[ASL][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ZPX shift")
    {
        // X=1, mem[0x11]=0x01, ASL $10,X => 0x02
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0x16, 0x10,
            0xB5, 0x10
        });
        emu.execute(12);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }
}

TEST_CASE("ASL with ABS addressing", "[ASL][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABS shift")
    {
        // mem[0x300]=0x01, ASL $0300 => 0x02
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0x0E, 0x00, 0x03,
            0xAD, 0x00, 0x03
        });
        emu.execute(10);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }
}

TEST_CASE("ASL with ABSX addressing", "[ASL][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABSX shift")
    {
        // X=1, mem[0x301]=0x01, ASL $0300,X => 0x02
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0x1E, 0x00, 0x03,
            0xBD, 0x00, 0x03
        });
        emu.execute(13);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }
}

#pragma endregion

#pragma region LSR Tests

// ==================== LSR ====================

TEST_CASE("LSR flag behaviour", "[LSR]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Sets carry flag when bit 0 was set")
    {
        // A=0x01 -> LSR -> 0x00, C=1, Z=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x01,
            0x4A
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x00);
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Clears carry flag when bit 0 was clear")
    {
        // A=0x02 -> LSR -> 0x01, C=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x02,
            0x4A
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
    }

    SECTION("Negative flag is always cleared")
    {
        // A=0x80 -> LSR -> 0x40, N=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x80,
            0x4A
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x40);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }
}

TEST_CASE("LSR with ZP addressing", "[LSR][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Basic shift")
    {
        // mem[0x10]=0x02, LSR $10 => 0x01
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0x46, 0x10,
            0xA5, 0x10
        });
        emu.execute(8);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }
}

TEST_CASE("LSR with ZPX addressing", "[LSR][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ZPX shift")
    {
        // X=1, mem[0x11]=0x02, LSR $10,X => 0x01
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0x56, 0x10,
            0xB5, 0x10
        });
        emu.execute(12);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }
}

TEST_CASE("LSR with ABS addressing", "[LSR][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABS shift")
    {
        // mem[0x300]=0x02, LSR $0300 => 0x01
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0x4E, 0x00, 0x03,
            0xAD, 0x00, 0x03
        });
        emu.execute(10);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }
}

TEST_CASE("LSR with ABSX addressing", "[LSR][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABSX shift")
    {
        // X=1, mem[0x301]=0x02, LSR $0300,X => 0x01
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0x5E, 0x00, 0x03,
            0xBD, 0x00, 0x03
        });
        emu.execute(13);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }
}

#pragma endregion

#pragma region ROL Tests

// ==================== ROL ====================

TEST_CASE("ROL flag behaviour", "[ROL]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Carry in becomes bit 0")
    {
        // SEC, A=0x00 -> ROL -> 0x01 (old carry rotated into bit 0), C=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0x38,
            0xA9, 0x00,
            0x2A
        });
        emu.execute(6);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
    }

    SECTION("Sets carry flag when bit 7 was set")
    {
        // A=0x80, carry in clear -> ROL -> 0x00, C=1, Z=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x80,
            0x2A
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x00);
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Sets negative flag")
    {
        // A=0x40, carry in clear -> ROL -> 0x80, N=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x40,
            0x2A
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x80);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }
}

TEST_CASE("ROL with ZP addressing", "[ROL][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Basic rotate")
    {
        // mem[0x10]=0x01, ROL $10 => 0x02 (carry in clear)
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0x26, 0x10,
            0xA5, 0x10
        });
        emu.execute(8);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }
}

TEST_CASE("ROL with ZPX addressing", "[ROL][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ZPX rotate")
    {
        // X=1, mem[0x11]=0x01, ROL $10,X => 0x02
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0x36, 0x10,
            0xB5, 0x10
        });
        emu.execute(12);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }
}

TEST_CASE("ROL with ABS addressing", "[ROL][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABS rotate")
    {
        // mem[0x300]=0x01, ROL $0300 => 0x02
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0x2E, 0x00, 0x03,
            0xAD, 0x00, 0x03
        });
        emu.execute(10);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }
}

TEST_CASE("ROL with ABSX addressing", "[ROL][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABSX rotate")
    {
        // X=1, mem[0x301]=0x01, ROL $0300,X => 0x02
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0x3E, 0x00, 0x03,
            0xBD, 0x00, 0x03
        });
        emu.execute(13);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }
}

#pragma endregion

#pragma region ROR Tests

// ==================== ROR ====================

TEST_CASE("ROR flag behaviour", "[ROR]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Carry in becomes bit 7")
    {
        // SEC, A=0x00 -> ROR -> 0x80 (old carry rotated into bit 7), C=0, N=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0x38,
            0xA9, 0x00,
            0x6A
        });
        emu.execute(6);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x80);
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Sets carry flag when bit 0 was set")
    {
        // A=0x01, carry in clear -> ROR -> 0x00, C=1, Z=1
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x01,
            0x6A
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x00);
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Clears carry flag when bit 0 was clear")
    {
        // A=0x02, carry in clear -> ROR -> 0x01, C=0
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x02,
            0x6A
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
    }
}

TEST_CASE("ROR with ZP addressing", "[ROR][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Basic rotate")
    {
        // mem[0x10]=0x02, ROR $10 => 0x01 (carry in clear)
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0x66, 0x10,
            0xA5, 0x10
        });
        emu.execute(8);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }
}

TEST_CASE("ROR with ZPX addressing", "[ROR][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ZPX rotate")
    {
        // X=1, mem[0x11]=0x02, ROR $10,X => 0x01
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0x76, 0x10,
            0xB5, 0x10
        });
        emu.execute(12);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }
}

TEST_CASE("ROR with ABS addressing", "[ROR][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABS rotate")
    {
        // mem[0x300]=0x02, ROR $0300 => 0x01
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0x6E, 0x00, 0x03,
            0xAD, 0x00, 0x03
        });
        emu.execute(10);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }
}

TEST_CASE("ROR with ABSX addressing", "[ROR][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABSX rotate")
    {
        // X=1, mem[0x301]=0x02, ROR $0300,X => 0x01
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0x7E, 0x00, 0x03,
            0xBD, 0x00, 0x03
        });
        emu.execute(13);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }
}

#pragma endregion
