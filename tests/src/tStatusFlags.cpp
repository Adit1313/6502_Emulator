#include "tests.h"

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

    SECTION("SEC does not affect other flags")
    {
        // SEC
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38});
        emu.execute(2);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::D) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::I) == 0);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), SEC (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0x38, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region CLC Tests

TEST_CASE("CLC tests", "[CLC][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify C flag is cleared")
    {
        // SEC, CLC → C cleared
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0x18});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
    }

    SECTION("CLC is idempotent")
    {
        // CLC, CLC → C remains clear
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x18, 0x18});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
    }

    SECTION("CLC does not affect other flags")
    {
        // SEC, CLC
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x38, 0x18});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::D) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::I) == 0);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), CLC (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0x18, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region SED Tests

TEST_CASE("SED tests", "[SED][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify D flag is set")
    {
        // SED
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xF8});
        emu.execute(2);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::D) == 1);
    }

    SECTION("SED is idempotent")
    {
        // SED, SED → D remains set
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xF8, 0xF8});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::D) == 1);
    }

    SECTION("SED does not affect other flags")
    {
        // SED
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xF8});
        emu.execute(2);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::I) == 0);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), SED (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xF8, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region CLD Tests

TEST_CASE("CLD tests", "[CLD][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify D flag is cleared")
    {
        // SED, CLD → D cleared
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xF8, 0xD8});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::D) == 0);
    }

    SECTION("CLD is idempotent")
    {
        // CLD, CLD → D remains clear
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xD8, 0xD8});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::D) == 0);
    }

    SECTION("CLD does not affect other flags")
    {
        // SED, CLD
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xF8, 0xD8});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::I) == 0);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), CLD (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xD8, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region SEI Tests

TEST_CASE("SEI tests", "[SEI][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify I flag is set")
    {
        // SEI
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x78});
        emu.execute(2);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::I) == 1);
    }

    SECTION("SEI is idempotent")
    {
        // SEI, SEI → I remains set
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x78, 0x78});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::I) == 1);
    }

    SECTION("SEI does not affect other flags")
    {
        // SEI
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x78});
        emu.execute(2);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::D) == 0);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), SEI (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0x78, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region CLI Tests

TEST_CASE("CLI tests", "[CLI][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify I flag is cleared")
    {
        // SEI, CLI → I cleared
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x78, 0x58});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::I) == 0);
    }

    SECTION("CLI is idempotent")
    {
        // CLI, CLI → I remains clear
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x58, 0x58});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::I) == 0);
    }

    SECTION("CLI does not affect other flags")
    {
        // SEI, CLI
        emu.load_bytes_at_address(0x200, std::vector<u8> {0x78, 0x58});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::D) == 0);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), CLI (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0x58, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region CLV Tests

TEST_CASE("CLV tests", "[CLV][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Verify V flag is cleared")
    {
        // LDA #0x7F, ADC #0x01 → A=0x80, V=1, then CLV → V cleared
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x7F,
            0x69, 0x01,
            0xB8
        });
        emu.execute(6);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
    }

    SECTION("CLV is idempotent")
    {
        // CLV, CLV → V remains clear
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xB8, 0xB8});
        emu.execute(4);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_flag(CPU_6502::V) == 0);
    }

    SECTION("CLV does not affect other flags")
    {
        // LDA #0x7F, ADC #0x01 → A=0x80, N=1, C=0, V=1, then CLV
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x7F,
            0x69, 0x01,
            0xB8
        });
        emu.execute(6);

        auto cpu = emu.get_CPU_obj();
        REQUIRE(cpu.get_CPU_state().A == 0x80);
        REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
        REQUIRE(cpu.get_flag(CPU_6502::C) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::Z) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::D) == 0);
        REQUIRE(cpu.get_flag(CPU_6502::I) == 0);
    }

    SECTION("Verify cycle count")
    {
        // LDA #0x00 (marker baseline, 2), CLV (tested, 2), LDA #0xEE (marker)
        emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x00, 0xB8, 0xA9, 0xEE});
        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion