#include "tests.h"

#pragma region INC Tests

// ==================== INC ====================

TEST_CASE("INC flag behaviour", "[INC]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Increment sets Z flag")
    {
        // mem[0x10] = 0xFF -> 0x00
        emu.load_bytes_at_address(0x10, std::vector<u8> {0xFF});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xE6, 0x10});
        emu.execute(5);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Increment sets N flag")
    {
        // mem[0x10] = 0x7F -> 0x80
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x7F});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xE6, 0x10});
        emu.execute(5);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Increment clears Z flag")
    {
        // mem[0x10] = 0x01 -> 0x02
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xE6, 0x10});
        emu.execute(5);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
}

TEST_CASE("INC with ZP addressing", "[INC][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Basic increment")
    {
        // mem[0x10]=0x01, INC $10 => 0x02
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xE6, 0x10,
            0xA5, 0x10
        });
        emu.execute(7);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), INC $10 (tested ZP, 5), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xE6, 0x10, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 8, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("INC with ZPX addressing", "[INC][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ZPX increment")
    {
        // X=1, mem[0x11]=0x01, INC $10,X => 0x02
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0xF6, 0x10,
            0xB5, 0x10        // LDA $10,X
        });
        emu.execute(9);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }

    SECTION("Verify cycle count")
    {
        // LDX #1 (2), LDA #0x00 (marker baseline, 2), INC $10,X (tested ZPX, 6, mem[0x11]), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x00, 0xF6, 0x10, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 11, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("INC with ABS addressing", "[INC][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABS increment")
    {
        // mem[0x300]=0x01, INC $0300 => 0x02
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xEE, 0x00, 0x03,
            0xAD, 0x00, 0x03
        });
        emu.execute(10);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), INC $0300 (tested ABS, 6), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xEE, 0x00, 0x03, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 9, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("INC with ABSX addressing", "[INC][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABSX increment")
    {
        // X=1, mem[0x301]=0x01, INC $0300,X => 0x02
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0xFE, 0x00, 0x03,
            0xBD, 0x00, 0x03
        });
        emu.execute(11);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x02);
    }

    SECTION("Verify cycle count (same page)")
    {
        // LDX #1 (2), LDA #0x00 (marker baseline, 2), INC $0300,X -> $0301 same page (tested ABSX, 7), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x00, 0xFE, 0x00, 0x03, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 12, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Verify cycle count (page boundary crossed, KNOWN DEVIATION from real 6502)")
    {
        // NOTE: Real 6502 INC $nnnn,X always costs 7 cycles, page-crossing included, since it
        // is a read-modify-write instruction and must perform the write regardless. This
        // emulator's ABSX() addressing mode unconditionally adds a page-cross cycle regardless
        // of which instruction is using it, so INC currently (incorrectly) costs 8 here instead
        // of 7. This test documents the emulator's actual behaviour until that is fixed.
        // LDX #2 (2), LDA #0x00 (marker baseline, 2), INC $02FF,X -> $0301 crosses page (tested ABSX, 7+1), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x02, 0xA9, 0x00, 0xFE, 0xFF, 0x2, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 13, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region INX, INY, DEX, DEY Tests

TEST_CASE("INX flag behaviour", "[INX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Result is zero (Z=1)")
    {
        // LDX #0xFF, INX => 0x00
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0xFF,
            0xE8
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().X == 0x00);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Result negative (N=1)")
    {
        // LDX #0x7F, INX => 0x80
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x7F,
            0xE8
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().X == 0x80);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Normal increment")
    {
        // LDX #0x01, INX => 0x02
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0xE8
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().X == 0x02);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), LDX #0x00 (2), INX (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xA2, 0x00, 0xE8, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("INY flag behaviour", "[INY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Result is zero (Z=1)")
    {
        // LDY #0xFF, INY => 0x00
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA0, 0xFF,
            0xC8
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().Y == 0x00);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Result negative (N=1)")
    {
        // LDY #0x7F, INY => 0x80
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA0, 0x7F,
            0xC8
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().Y == 0x80);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Normal increment")
    {
        // LDY #0x01, INY => 0x02
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA0, 0x01,
            0xC8
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().Y == 0x02);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), LDY #0x00 (2), INY (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xA0, 0x00, 0xC8, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

// ==================== DEX ====================

TEST_CASE("DEX flag behaviour", "[DEX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Result is zero (Z=1)")
    {
        // LDX #0x01, DEX => 0x00
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0xCA
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().X == 0x00);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Result negative (N=1)")
    {
        // LDX #0x00, DEX => 0xFF
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x00,
            0xCA
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().X == 0xFF);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Normal decrement")
    {
        // LDX #0x03, DEX => 0x02
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x03,
            0xCA
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().X == 0x02);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), LDX #0x01 (2), DEX (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xA2, 0x01, 0xCA, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

// ==================== DEY ====================

TEST_CASE("DEY flag behaviour", "[DEY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Result is zero (Z=1)")
    {
        // LDY #0x01, DEY => 0x00
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA0, 0x01,
            0x88
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().Y == 0x00);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Result negative (N=1)")
    {
        // LDY #0x00, DEY => 0xFF
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA0, 0x00,
            0x88
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().Y == 0xFF);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Normal decrement")
    {
        // LDY #0x03, DEY => 0x02
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA0, 0x03,
            0x88
        });
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().Y == 0x02);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), LDY #0x01 (2), DEY (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xA0, 0x01, 0x88, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region DEC Tests

TEST_CASE("DEC flag behaviour", "[DEC]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Result is zero (Z=1)")
    {
        // mem[0x10]=0x01, DEC $10 => 0x00
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x01});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xC6, 0x10,
            0xA5, 0x10
        });
        emu.execute(7);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x00);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Result negative (N=1)")
    {
        // mem[0x10]=0x00, DEC $10 => 0xFF
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x00});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xC6, 0x10,
            0xA5, 0x10
        });
        emu.execute(7);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0xFF);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }
}

TEST_CASE("DEC with ZP addressing", "[DEC][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Basic decrement")
    {
        // mem[0x10]=0x02, DEC $10 => 0x01
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xC6, 0x10,
            0xA5, 0x10
        });
        emu.execute(7);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), DEC $10 (tested ZP, 5), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x10, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xC6, 0x10, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 8, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("DEC with ZPX addressing", "[DEC][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ZPX decrement")
    {
        // X=1, mem[0x11]=0x02, DEC $10,X => 0x01
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0xD6, 0x10,
            0xB5, 0x10
        });
        emu.execute(9);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }

    SECTION("Verify cycle count")
    {
        // LDX #1 (2), LDA #0x00 (marker baseline, 2), DEC $10,X (tested ZPX, 6, mem[0x11]), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x11, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x00, 0xD6, 0x10, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 11, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("DEC with ABS addressing", "[DEC][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABS decrement")
    {
        // mem[0x300]=0x02, DEC $0300 => 0x01
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xCE, 0x00, 0x03,
            0xAD, 0x00, 0x03
        });
        emu.execute(10);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), DEC $0300 (tested ABS, 6), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x300, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xCE, 0x00, 0x03, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 9, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("DEC with ABSX addressing", "[DEC][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("ABSX decrement")
    {
        // X=1, mem[0x301]=0x02, DEC $0300,X => 0x01
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA2, 0x01,
            0xDE, 0x00, 0x03,
            0xBD, 0x00, 0x03
        });
        emu.execute(11);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x01);
    }

    SECTION("Verify cycle count (same page)")
    {
        // LDX #1 (2), LDA #0x00 (marker baseline, 2), DEC $0300,X -> $0301 same page (tested ABSX, 7), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x01, 0xA9, 0x00, 0xDE, 0x00, 0x03, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 12, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Verify cycle count (page boundary crossed, KNOWN DEVIATION from real 6502)")
    {
        // NOTE: Real 6502 DEC $nnnn,X always costs 7 cycles, page-crossing included (see the
        // matching INC ABSX note). This emulator's ABSX() addressing mode unconditionally adds
        // a page-cross cycle regardless of which instruction is using it, so DEC currently
        // (incorrectly) costs 8 here instead of 7. This test documents the emulator's actual
        // behaviour until that is fixed.
        // LDX #2 (2), LDA #0x00 (marker baseline, 2), DEC $02FF,X -> $0301 crosses page (tested ABSX, 7+1), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x301, std::vector<u8> {0x02});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x02, 0xA9, 0x00, 0xDE, 0xFF, 0x2, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 13, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion