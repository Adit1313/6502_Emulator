#include "tests.h"

TEST_CASE("TAX tests", "[Transfer][TXA][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F, 0xAA});
    emu.reset(emu.RST_CPU);

    SECTION("Verify register X value")
    {
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.X == 0x8F);
    }

    SECTION("Verify N flag set")
    {
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag cleared")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x1, 0xA0, 0x8F, 0xAA});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify Z flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x0, 0xAA});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag cleared")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x1, 0xA0, 0x0, 0xAA});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }

    SECTION("Verify cycle count")
    {
        // LDX #0x00 (X baseline, 2), LDA #0x8F (value to transfer, 2), TAX (tested, 2), LDX #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x00, 0xA9, 0x8F, 0xAA, 0xA2, 0xEE});
        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().X; });
    }

}

TEST_CASE("TAY tests", "[Transfer][TYA][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F, 0xA8});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify register Y value")
    {
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.Y == 0x8F);
    }

    SECTION("Verify N flag set")
    {
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag cleared")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x1, 0xA0, 0x8F, 0xA8});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify Z flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x0, 0xA8});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag cleared")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x1, 0xA0, 0x0, 0xA8});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }

    SECTION("Verify cycle count")
    {
        // LDY #0x00 (Y baseline, 2), LDA #0x8F (value to transfer, 2), TAY (tested, 2), LDY #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x00, 0xA9, 0x8F, 0xA8, 0xA0, 0xEE});
        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().Y; });
    }

}

TEST_CASE("TXA tests", "[Transfer][TXA][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F, 0x8A});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify accumulator value")
    {
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.A == 0x8F);
    }

    SECTION("Verify N flag set")
    {
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag cleared")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x1, 0xA0, 0x8F, 0x8A});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify Z flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x0, 0x8A});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag cleared")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x1, 0xA0, 0x0, 0x8A});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (A baseline, 2), LDX #0x8F (value to transfer, 2), TXA (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xA2, 0x8F, 0x8A, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

}

TEST_CASE("TYA tests", "[Transfer][TYA][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x8F, 0x98});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify accumulator value")
    {
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(state.A == 0x8F);
    }

    SECTION("Verify N flag set")
    {
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    }

    SECTION("Verify N flag cleared")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x1, 0xA2, 0x8F, 0x98});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
    }

    SECTION("Verify Z flag set")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x0, 0x98});
        emu.execute(4);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    }

    SECTION("Verify Z flag cleared")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0x1, 0xA2, 0x0, 0x98});
        emu.execute(6);
        auto cpu = emu.get_CPU_obj();
        auto state = cpu.get_CPU_state();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (A baseline, 2), LDY #0x8F (value to transfer, 2), TYA (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xA0, 0x8F, 0x98, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

}