#include "tests.h"

#pragma region BCC Tests

TEST_CASE("BCC branch behaviour", "[BCC]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Taken when carry clear (2 base + 1 taken = 3 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x99,         // 0x200 LDA #0x99 (sentinel, carry untouched, C=0 default)
            0x90, 0x02,         // 0x202 BCC +2 -> target 0x206
            0xA9, 0xEE,         // 0x204 LDA #0xEE (fallthrough, must be skipped)
            0xA9, 0x11          // 0x206 LDA #0x11 (target)
        });

        require_exact_cycle_count(emu, 6, 0x11, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Not taken when carry set (2 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x99,         // 0x200 LDA #0x99 (sentinel)
            0x38,               // 0x202 SEC (C=1)
            0x90, 0x02,         // 0x203 BCC +2 -> not taken
            0xA9, 0xEE          // 0x205 LDA #0xEE (fallthrough, must execute)
        });

        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region BCS Tests

TEST_CASE("BCS branch behaviour", "[BCS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Taken when carry set (3 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x99,         // 0x200 LDA #0x99 (sentinel)
            0x38,               // 0x202 SEC (C=1)
            0xB0, 0x02,         // 0x203 BCS +2 -> target 0x207
            0xA9, 0xEE,         // 0x205 LDA #0xEE (fallthrough, must be skipped)
            0xA9, 0x11          // 0x207 LDA #0x11 (target)
        });

        require_exact_cycle_count(emu, 8, 0x11, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Not taken when carry clear (2 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x99,         // 0x200 LDA #0x99 (sentinel, C=0 default)
            0xB0, 0x02,         // 0x202 BCS +2 -> not taken
            0xA9, 0xEE          // 0x204 LDA #0xEE (fallthrough, must execute)
        });

        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region BEQ Tests

TEST_CASE("BEQ branch behaviour", "[BEQ]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Taken when zero flag set (3 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x00,         // 0x200 LDA #0x00 (Z=1)
            0xF0, 0x02,         // 0x202 BEQ +2 -> target 0x206
            0xA9, 0xEE,         // 0x204 LDA #0xEE (fallthrough, must be skipped)
            0xA9, 0x11          // 0x206 LDA #0x11 (target)
        });

        require_exact_cycle_count(emu, 6, 0x11, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Not taken when zero flag clear (2 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x01,         // 0x200 LDA #0x01 (Z=0)
            0xF0, 0x02,         // 0x202 BEQ +2 -> not taken
            0xA9, 0xEE          // 0x204 LDA #0xEE (fallthrough, must execute)
        });

        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region BNE Tests

TEST_CASE("BNE branch behaviour", "[BNE]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Taken when zero flag clear (3 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x01,         // 0x200 LDA #0x01 (Z=0)
            0xD0, 0x02,         // 0x202 BNE +2 -> target 0x206
            0xA9, 0xEE,         // 0x204 LDA #0xEE (fallthrough, must be skipped)
            0xA9, 0x11          // 0x206 LDA #0x11 (target)
        });

        require_exact_cycle_count(emu, 6, 0x11, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Not taken when zero flag set (2 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x00,         // 0x200 LDA #0x00 (Z=1)
            0xD0, 0x02,         // 0x202 BNE +2 -> not taken
            0xA9, 0xEE          // 0x204 LDA #0xEE (fallthrough, must execute)
        });

        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region BMI Tests

TEST_CASE("BMI branch behaviour", "[BMI]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Taken when negative flag set (3 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x80,         // 0x200 LDA #0x80 (N=1)
            0x30, 0x02,         // 0x202 BMI +2 -> target 0x206
            0xA9, 0xEE,         // 0x204 LDA #0xEE (fallthrough, must be skipped)
            0xA9, 0x11          // 0x206 LDA #0x11 (target)
        });

        require_exact_cycle_count(emu, 6, 0x11, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Not taken when negative flag clear (2 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x01,         // 0x200 LDA #0x01 (N=0)
            0x30, 0x02,         // 0x202 BMI +2 -> not taken
            0xA9, 0xEE          // 0x204 LDA #0xEE (fallthrough, must execute)
        });

        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region BPL Tests

TEST_CASE("BPL branch behaviour", "[BPL]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Taken when negative flag clear (3 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x01,         // 0x200 LDA #0x01 (N=0)
            0x10, 0x02,         // 0x202 BPL +2 -> target 0x206
            0xA9, 0xEE,         // 0x204 LDA #0xEE (fallthrough, must be skipped)
            0xA9, 0x11          // 0x206 LDA #0x11 (target)
        });

        require_exact_cycle_count(emu, 6, 0x11, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Not taken when negative flag set (2 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x80,         // 0x200 LDA #0x80 (N=1)
            0x10, 0x02,         // 0x202 BPL +2 -> not taken
            0xA9, 0xEE          // 0x204 LDA #0xEE (fallthrough, must execute)
        });

        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region BVC Tests

TEST_CASE("BVC branch behaviour", "[BVC]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Taken when overflow flag clear (3 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x99,         // 0x200 LDA #0x99 (sentinel, V=0 default)
            0x50, 0x02,         // 0x202 BVC +2 -> target 0x206
            0xA9, 0xEE,         // 0x204 LDA #0xEE (fallthrough, must be skipped)
            0xA9, 0x11          // 0x206 LDA #0x11 (target)
        });

        require_exact_cycle_count(emu, 6, 0x11, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Not taken when overflow flag set (2 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x7F,         // 0x200 LDA #0x7F
            0x69, 0x01,         // 0x202 ADC #0x01 -> A=0x80, V=1 (sentinel doubles as overflow setup)
            0x50, 0x02,         // 0x204 BVC +2 -> not taken
            0xA9, 0xEE          // 0x206 LDA #0xEE (fallthrough, must execute)
        });

        require_exact_cycle_count(emu, 7, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region BVS Tests

TEST_CASE("BVS branch behaviour", "[BVS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    SECTION("Taken when overflow flag set (3 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x7F,         // 0x200 LDA #0x7F
            0x69, 0x01,         // 0x202 ADC #0x01 -> A=0x80, V=1
            0x70, 0x02,         // 0x204 BVS +2 -> target 0x208
            0xA9, 0xEE,         // 0x206 LDA #0xEE (fallthrough, must be skipped)
            0xA9, 0x11          // 0x208 LDA #0x11 (target)
        });

        require_exact_cycle_count(emu, 8, 0x11, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }

    SECTION("Not taken when overflow flag clear (2 cycles)")
    {
        emu.load_bytes_at_address(0x200, std::vector<u8> {
            0xA9, 0x99,         // 0x200 LDA #0x99 (sentinel, V=0 default)
            0x70, 0x02,         // 0x202 BVS +2 -> not taken
            0xA9, 0xEE          // 0x204 LDA #0xEE (fallthrough, must execute)
        });

        require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
    }
}

#pragma endregion

#pragma region Page Boundary and Direction Coverage

TEST_CASE("Branch taken across a page boundary costs an extra cycle", "[BCC][PageCross]")
{
    Emulator emu;
    // Start execution directly at 0x2FA (rather than the usual 0x0200) so
    // the branch lands close to a page boundary.
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0xFA, 0x2});
    emu.reset(emu.RST_CPU);

    // PC lands on 0x2FE after the operand fetch; +5 crosses into page 0x03.
    emu.load_bytes_at_address(0x2FA, std::vector<u8> {
        0xA9, 0x99,             // 0x2FA LDA #0x99 (sentinel, C=0 default)
        0x90, 0x05              // 0x2FC BCC +5 -> target 0x303 (page cross)
    });
    emu.load_bytes_at_address(0x303, std::vector<u8> {0xA9, 0x11}); // LDA #0x11 (target)

    // 2 (sentinel) + 4 (2 base + 1 taken + 1 page-cross) + 1 = 7; without the
    // page-cross cycle the target would already be visible one tick sooner.
    require_exact_cycle_count(emu, 7, 0x11, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
}

TEST_CASE("Branch supports negative (backward) offsets", "[BCC][Backward]")
{
    Emulator emu;
    // Start execution at 0x210 so the backward target at 0x200 is only
    // reached via the branch, never by falling into it sequentially.
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x10, 0x2});
    emu.reset(emu.RST_CPU);

    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x11}); // LDA #0x11 (backward target)
    emu.load_bytes_at_address(0x210, std::vector<u8> {
        0xA9, 0x99,             // 0x210 LDA #0x99 (sentinel, C=0 default)
        0x90, 0xEC              // 0x212 BCC -20 -> target 0x200 (same page, no page cross)
    });

    // 2 (sentinel) + 3 (2 base + 1 taken, no page cross) + 1 = 6.
    require_exact_cycle_count(emu, 6, 0x11, [&]{ return emu.get_CPU_obj().get_CPU_state().A; });
}

#pragma endregion
