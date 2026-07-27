#include "tests.h"

TEST_CASE("TSX tests","[TSX][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xBA});
    /*
    Test does the following:
    1. Load the accumulator
    2. Push it onto the stack
    3. Verify that value is stack memory
    4. Load a negative value to Y
    5. Push flags to stack
    6. Verify that value is in stack memory
    7. Pull
    */
    emu.reset(emu.RST_CPU);
    emu.execute(2);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    REQUIRE(state.X == 0xFD);
}

TEST_CASE("TSX cycle count","[TSX][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    // LDX #0x00 (X baseline, 2), TSX (tested, 2, X <- SP == 0xFD), LDX #0xEE (marker)
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x00, 0xBA, 0xA2, 0xEE});
    require_exact_cycle_count(emu, 5, 0xEE, [&]{ return emu.get_CPU_obj().get_CPU_state().X; });
}

TEST_CASE("TXS tests","[TXS][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0xFE, 0x9A});
    emu.reset(emu.RST_CPU);
    emu.execute(4);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    REQUIRE(state.SP == 0xFE);
}

TEST_CASE("TXS cycle count","[TXS][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    // LDX #0xFE (value to transfer, 2), TXS (tested, 2, SP <- 0xFE), LDX #0x11 + TXS (marker: SP <- 0x11)
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0xFE, 0x9A, 0xA2, 0x11, 0x9A});
    require_exact_cycle_count(emu, 7, 0x11, [&]{ return emu.get_CPU_obj().get_CPU_state().SP; });
}

TEST_CASE("PHA tests","[PHA][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x23, 0x48, 0xAC, 0xFD, 0x01});
    emu.reset(emu.RST_CPU);
    emu.execute(8);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    
    REQUIRE(state.SP == 0xFC);  // Check if SP decremented
    REQUIRE(state.Y == 0x23);   // Check if value in stack is correct
}

TEST_CASE("PHA cycle count","[PHA][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    // SP starts at 0xFD (known baseline). PHA (tested, 3, SP -> 0xFC), PHP (marker, SP -> 0xFB)
    emu.load_bytes_at_address(0x200, std::vector<u8> {0x48, 0x08});
    require_exact_cycle_count(emu, 4, 0xFB, [&]{ return emu.get_CPU_obj().get_CPU_state().SP; });
}

TEST_CASE("PHP tests","[PHP][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F, 0x08, 0xAC, 0xFD, 0x01});
    emu.reset(emu.RST_CPU);
    emu.execute(9);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    
    REQUIRE(state.SP == 0xFC);  // Check if SP decremented
    REQUIRE(GET_BIT(state.Y, emu.get_CPU_obj().N) == 1);   // Check if value in stack is correct
}

TEST_CASE("PHP cycle count","[PHP][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    // SP starts at 0xFD (known baseline). PHP (tested, 3, SP -> 0xFC), PHA (marker, SP -> 0xFB)
    emu.load_bytes_at_address(0x200, std::vector<u8> {0x08, 0x48});
    require_exact_cycle_count(emu, 4, 0xFB, [&]{ return emu.get_CPU_obj().get_CPU_state().SP; });
}

TEST_CASE("PLA tests","[PLA][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x23, 0x48, 0xA9, 0x21, 0x48, 0x68, 0x68});
    emu.reset(emu.RST_CPU);
    emu.execute(18);

    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    REQUIRE(state.SP == 0xFD);  // Check if SP back at start
    REQUIRE(state.A == 0x23);   // Check if value in stack is correct
}

TEST_CASE("PLA cycle count","[PLA][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    // SP starts at 0xFD (known baseline). PLA (tested, 4, SP -> 0xFE), PLA (marker, SP -> 0xFF)
    emu.load_bytes_at_address(0x200, std::vector<u8> {0x68, 0x68});
    require_exact_cycle_count(emu, 5, 0xFF, [&]{ return emu.get_CPU_obj().get_CPU_state().SP; });
}

TEST_CASE("PLP tests","[PLP][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F, 0x08, 0xA2, 0x0, 0x08, 0xA2, 0x1, 0x28, 0x28});
    emu.reset(emu.RST_CPU);
    emu.execute(16);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    emu.execute(4);
    cpu = emu.get_CPU_obj();
    state = cpu.get_CPU_state();
    REQUIRE(state.SP == 0xFD);
    REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
}

TEST_CASE("PLP cycle count","[PLP][IMP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2});
    emu.reset(emu.RST_CPU);

    // SP starts at 0xFD (known baseline). PLP (tested, 4, SP -> 0xFE), PLP (marker, SP -> 0xFF)
    emu.load_bytes_at_address(0x200, std::vector<u8> {0x28, 0x28});
    require_exact_cycle_count(emu, 5, 0xFF, [&]{ return emu.get_CPU_obj().get_CPU_state().SP; });
}