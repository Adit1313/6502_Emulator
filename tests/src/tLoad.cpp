#include "tests.h"

TEST_CASE("LDA flag behaviour", "[LDA]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.reset(emu.RST_CPU);

    SECTION("Verify N flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag clear")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F, 0xA9, 0x2});
        emu.execute(4); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify Z flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x0});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag clear")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x0, 0xA9, 0x1});
        emu.execute(4); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
}

TEST_CASE("LDA with IMM addressing", "[LDA][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.A == 0x8F);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (sentinel, 2), LDA #0x8F (tested IMM, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xA9, 0x8F, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("LDA with ZP addressing", "[LDA][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA5, 0x20});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x20, std::vector<u8> {0xA5});
        emu.execute(3);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.A == 0xA5);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (sentinel, 2), LDA $20 (tested ZP, 3, mem=0xA5), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x20, std::vector<u8> {0xA5});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xA5, 0x20, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 6, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("LDA with ZPX addressing", "[LDA][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x1, 0xB5, 0x20});
    /*
    LDX #1
    LDA $20,X
    */
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x21, std::vector<u8> {0xA5});
        emu.execute(5);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.A == 0xA5);
    }

    SECTION("Verify cycle count")
    {
        // LDX #1 (2), LDA #0x00 sentinel (2), LDA $20,X (tested ZPX, 4, mem[0x21]=0xA5), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x21, std::vector<u8> {0xA5});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x1, 0xA9, 0x00, 0xB5, 0x20, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 9, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("LDA with ABS addressing", "[LDA][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xAD, 0x34, 0x12});
    emu.load_bytes_at_address(0x1234, std::vector<u8> {0x69});
    /*
    LDX #1
    LDA $20,X
    */
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.A == 0x69);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (sentinel, 2), LDA $1234 (tested ABS, 4), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xAD, 0x34, 0x12, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("LDA with ABSX addressing", "[LDA][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x5, 0xBD, 0x34, 0x12});
    emu.load_bytes_at_address(0x1239, std::vector<u8> {0x69});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.A == 0x69);
    }

    SECTION("Verify cycle count (same page)")
    {
        // LDX #5 (2), LDA #0x00 sentinel (2), LDA $1234,X -> $1239 same page (tested ABSX, 4), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x5, 0xA9, 0x00, 0xBD, 0x34, 0x12, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 9, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Verify cycle count (page boundary crossed)")
    {
        // LDX #0xFF (2), LDA #0x00 sentinel (2), LDA $1234,X -> $1333 crosses page (tested ABSX, 5), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x1333, std::vector<u8> {0x69});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0xFF, 0xA9, 0x00, 0xBD, 0x34, 0x12, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 10, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("LDA with ABSY addressing", "[LDA][ABSY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x5, 0xB9, 0x34, 0x12});
    emu.load_bytes_at_address(0x1239, std::vector<u8> {0x69});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.A == 0x69);
    }

    SECTION("Verify cycle count (same page)")
    {
        // LDY #5 (2), LDA #0x00 sentinel (2), LDA $1234,Y -> $1239 same page (tested ABSY, 4), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x5, 0xA9, 0x00, 0xB9, 0x34, 0x12, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 9, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Verify cycle count (page boundary crossed)")
    {
        // LDY #0xFF (2), LDA #0x00 sentinel (2), LDA $1234,Y -> $1333 crosses page (tested ABSY, 5), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x1333, std::vector<u8> {0x69});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0xFF, 0xA9, 0x00, 0xB9, 0x34, 0x12, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 10, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("LDA with IZX addressing", "[LDA][IZX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x5, 0xA1, 0x20});
    emu.load_bytes_at_address(0x25, std::vector<u8> {0x34, 0x12});
    emu.load_bytes_at_address(0x1234, std::vector<u8> {0x69});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.A == 0x69);
    }

    SECTION("Verify page wrap around")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x26, 0xA1, 0xFF});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.A == 0x69);
    }

    SECTION("Verify cycle count")
    {
        // IZX is fixed-cost (no page-cross variability): LDX #5 (2), LDA #0x00 sentinel (2),
        // LDA ($20,X) (tested IZX, 6), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x5, 0xA9, 0x00, 0xA1, 0x20, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 11, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("LDA with IZY addressing", "[LDA][IZY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x3, 0xB1, 0x20});
    emu.load_bytes_at_address(0x20, std::vector<u8> {0x31, 0x12});
    emu.load_bytes_at_address(0x1234, std::vector<u8> {0x69});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.A == 0x69);
    }

    SECTION("Verify cycle count (same page)")
    {
        // ptr at 0x20 -> 0x1231, Y=3 (2) => 0x1234 same page, LDA #0x00 sentinel (2),
        // LDA ($20),Y (tested IZY, 5), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x20, std::vector<u8> {0x31, 0x12});
        emu.load_bytes_at_address(0x1234, std::vector<u8> {0x69});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x3, 0xA9, 0x00, 0xB1, 0x20, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 10, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Verify cycle count (page boundary crossed)")
    {
        // ptr at 0x20 -> 0x12FF, Y=3 (2) => 0x1302 crosses page, LDA #0x00 sentinel (2),
        // LDA ($20),Y (tested IZY, 6), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x20, std::vector<u8> {0xFF, 0x12});
        emu.load_bytes_at_address(0x1302, std::vector<u8> {0x69});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x3, 0xA9, 0x00, 0xB1, 0x20, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 11, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

TEST_CASE("LDX flag behaviour", "[LDX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.reset(emu.RST_CPU);

    SECTION("Verify N flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag clear")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F, 0xA2, 0x2});
        emu.execute(4); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify Z flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x0});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag clear")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x0, 0xA2, 0x1});
        emu.execute(4); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
}

TEST_CASE("LDX with IMM addressing", "[LDX][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.X == 0x8F);
    }

    SECTION("Verify cycle count")
    {
        // LDX #0x00 (sentinel, 2), LDX #0x8F (tested IMM, 2), LDX #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x00, 0xA2, 0x8F, 0xA2, 0xEE});
        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().X; });
    }
}

TEST_CASE("LDX with ZP addressing", "[LDX][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA6, 0x20});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x20, std::vector<u8> {0xA5});
        emu.execute(3);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.X == 0xA5);
    }

    SECTION("Verify cycle count")
    {
        // LDX #0x00 (sentinel, 2), LDX $20 (tested ZP, 3, mem=0xA5), LDX #0xEE (marker)
        emu.load_bytes_at_address(0x20, std::vector<u8> {0xA5});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x00, 0xA6, 0x20, 0xA2, 0xEE});
        require_exact_cycle_count(emu, 6, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().X; });
    }
}

TEST_CASE("LDX with ZPY addressing", "[LDX][ZPY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x1, 0xB6, 0x20});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x21, std::vector<u8> {0xA5});
        emu.execute(5);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.X == 0xA5);
    }

    SECTION("Verify cycle count")
    {
        // LDY #1 (2), LDX #0x00 sentinel (2), LDX $20,Y (tested ZPY, 4, mem[0x21]=0xA5), LDX #0xEE (marker)
        emu.load_bytes_at_address(0x21, std::vector<u8> {0xA5});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x1, 0xA2, 0x00, 0xB6, 0x20, 0xA2, 0xEE});
        require_exact_cycle_count(emu, 9, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().X; });
    }
}

TEST_CASE("LDX with ABS addressing", "[LDX][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xAE, 0x34, 0x12});
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
        auto state = cpu.get_CPU_state();
        REQUIRE(state.X == 0x69);
    }

    SECTION("Verify cycle count")
    {
        // LDX #0x00 (sentinel, 2), LDX $1234 (tested ABS, 4), LDX #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x00, 0xAE, 0x34, 0x12, 0xA2, 0xEE});
        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().X; });
    }
}

TEST_CASE("LDX with ABSY addressing", "[LDX][ABSY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x5, 0xBE, 0x34, 0x12});
    emu.load_bytes_at_address(0x1239, std::vector<u8> {0x69});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.X == 0x69);
    }

    SECTION("Verify cycle count (same page)")
    {
        // LDY #5 (2), LDX #0x00 sentinel (2), LDX $1234,Y -> $1239 same page (tested ABSY, 4), LDX #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x5, 0xA2, 0x00, 0xBE, 0x34, 0x12, 0xA2, 0xEE});
        require_exact_cycle_count(emu, 9, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().X; });
    }

    SECTION("Verify cycle count (page boundary crossed)")
    {
        // LDY #0xFF (2), LDX #0x00 sentinel (2), LDX $1234,Y -> $1333 crosses page (tested ABSY, 5), LDX #0xEE (marker)
        emu.load_bytes_at_address(0x1333, std::vector<u8> {0x69});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0xFF, 0xA2, 0x00, 0xBE, 0x34, 0x12, 0xA2, 0xEE});
        require_exact_cycle_count(emu, 10, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().X; });
    }
}

TEST_CASE("LDY flag behaviour", "[LDY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.reset(emu.RST_CPU);

    SECTION("Verify N flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x8F});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag clear")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x8F, 0xA0, 0x2});
        emu.execute(4); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify Z flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x0});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag clear")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x0, 0xA0, 0x1});
        emu.execute(4); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }
}

TEST_CASE("LDY with IMM addressing", "[LDY][IMM]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x8F});
        emu.execute(2); // Execute 2 steps
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.Y == 0x8F);
    }

    SECTION("Verify cycle count")
    {
        // LDY #0x00 (sentinel, 2), LDY #0x8F (tested IMM, 2), LDY #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x00, 0xA0, 0x8F, 0xA0, 0xEE});
        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().Y; });
    }
}

TEST_CASE("LDY with ZP addressing", "[LDY][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA4, 0x20});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x20, std::vector<u8> {0xA5});
        emu.execute(3);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.Y == 0xA5);
    }

    SECTION("Verify cycle count")
    {
        // LDY #0x00 (sentinel, 2), LDY $20 (tested ZP, 3, mem=0xA5), LDY #0xEE (marker)
        emu.load_bytes_at_address(0x20, std::vector<u8> {0xA5});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x00, 0xA4, 0x20, 0xA0, 0xEE});
        require_exact_cycle_count(emu, 6, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().Y; });
    }
}

TEST_CASE("LDY with ZPX addressing", "[LDY][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x1, 0xB4, 0x20});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.load_bytes_at_address(0x21, std::vector<u8> {0xA5});
        emu.execute(5);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.Y == 0xA5);
    }

    SECTION("Verify cycle count")
    {
        // LDX #1 (2), LDY #0x00 sentinel (2), LDY $20,X (tested ZPX, 4, mem[0x21]=0xA5), LDY #0xEE (marker)
        emu.load_bytes_at_address(0x21, std::vector<u8> {0xA5});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x1, 0xA0, 0x00, 0xB4, 0x20, 0xA0, 0xEE});
        require_exact_cycle_count(emu, 9, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().Y; });
    }
}

TEST_CASE("LDY with ABS addressing", "[LDY][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xAC, 0x34, 0x12});
    emu.load_bytes_at_address(0x1234, std::vector<u8> {0x69});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.Y == 0x69);
    }

    SECTION("Verify cycle count")
    {
        // LDY #0x00 (sentinel, 2), LDY $1234 (tested ABS, 4), LDY #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x00, 0xAC, 0x34, 0x12, 0xA0, 0xEE});
        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().Y; });
    }
}

TEST_CASE("LDY with ABSX addressing", "[LDY][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x5, 0xBC, 0x34, 0x12});
    emu.load_bytes_at_address(0x1239, std::vector<u8> {0x69});
    emu.reset(emu.RST_CPU);

    SECTION("Verify accumulator value")
    {
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.Y == 0x69);
    }

    SECTION("Verify cycle count (same page)")
    {
        // LDX #5 (2), LDY #0x00 sentinel (2), LDY $1234,X -> $1239 same page (tested ABSX, 4), LDY #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x5, 0xA0, 0x00, 0xBC, 0x34, 0x12, 0xA0, 0xEE});
        require_exact_cycle_count(emu, 9, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().Y; });
    }

    SECTION("Verify cycle count (page boundary crossed)")
    {
        // LDX #0xFF (2), LDY #0x00 sentinel (2), LDY $1234,X -> $1333 crosses page (tested ABSX, 5), LDY #0xEE (marker)
        emu.load_bytes_at_address(0x1333, std::vector<u8> {0x69});
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0xFF, 0xA0, 0x00, 0xBC, 0x34, 0x12, 0xA0, 0xEE});
        require_exact_cycle_count(emu, 10, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().Y; });
    }
}
