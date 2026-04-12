#include "emulator.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>

#pragma region SEC Tests

TEST_CASE("SEC tests", "[SEC][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify C flag is set")
    {
        // SEC
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38});
        emu.execute(2);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }

    SECTION("SEC is idempotent")
    {
        // SEC, SEC → C remains set
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0x38});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 1);
    }
}

#pragma endregion