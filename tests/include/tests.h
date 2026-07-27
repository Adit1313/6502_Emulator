#pragma once

#include "emulator.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>
#include <functional>

// Pins down the EXACT number of cycles an instruction takes, rather than
// merely "enough". CPU_6502::clock() executes an instruction's full effect
// on the very first tick its cycle counter reaches zero -- NOT on its last
// tick -- so a single instruction's own side effect appears one tick after
// everything before it finishes, regardless of how many cycles the
// instruction itself is supposed to cost. That means verifying instruction
// K's cycle count requires looking at when instruction K+1 fires, not at
// K's own value change.
//
// So the loaded program must have three parts:
//   1. Setup: any instructions establishing a known "before" state (e.g. a
//      sentinel load, plus any index-register setup the addressing mode
//      under test needs). Let S = the total cycles these take.
//   2. The instruction under test, whose cycle count T you want to verify.
//   3. A marker instruction immediately after it, changing `read_state()`
//      to `final_value` (a value distinct from whatever the tested
//      instruction itself produces).
//
// Call this with `total_cycles = S + T + 1`: `read_state()` must NOT yet
// equal `final_value` after `total_cycles - 1` ticks (the tested
// instruction has produced its own effect, but the marker hasn't fired
// yet), and MUST equal it after exactly one more tick (the marker's first
// tick). If the tested instruction actually costs more or fewer than T
// cycles, one of the two REQUIREs below fails.
inline void require_exact_cycle_count(
    Emulator& emu,
    u32 total_cycles,
    u8 final_value,
    const std::function<u8()>& read_state)
{
    emu.execute(total_cycles - 1);
    REQUIRE(read_state() != final_value);

    emu.execute(1);
    REQUIRE(read_state() == final_value);
}
