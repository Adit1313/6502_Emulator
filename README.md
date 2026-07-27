# 6502 Emulator

A cycle-aware emulator for the MOS 6502 microprocessor, written in modern C++17.

It models the CPU as a table-driven interpreter (opcode → operation + addressing mode + base cycle count), backed by a flat 64KB memory bus, and is developed test-first with [Catch2](https://github.com/catchorg/Catch2).

## Features

- Table-driven instruction dispatch (`opcode_table[256]`), matching the real 6502's opcode layout
- All 13 addressing modes: Implied, Accumulator, Immediate, Zero Page (+X/+Y), Absolute (+X/+Y), Indirect Indexed / Indexed Indirect, and Relative
- Accurate base cycle counts, plus the extra cycles the real chip spends on page-boundary crossings and taken branches
- Full status flag emulation (`C Z I D B U V N`)
- A minimal `Bus`/`Emulator` harness for loading programs and stepping the CPU cycle-by-cycle

### Implemented instructions

| Category | Instructions |
|---|---|
| Load / Store | `LDA` `LDX` `LDY` `STA` `STX` `STY` |
| Register Transfers | `TAX` `TAY` `TXA` `TYA` |
| Stack Operations | `TSX` `TXS` `PHA` `PHP` `PLA` `PLP` |
| Logical | `AND` `EOR` `ORA` `BIT` |
| Arithmetic | `ADC` `SBC` `CMP` `CPX` `CPY` |
| Increments / Decrements | `INC` `INX` `INY` `DEC` `DEX` `DEY` |
| Shifts | `ASL` `LSR` `ROL` `ROR` |
| Branches | `BCC` `BCS` `BEQ` `BMI` `BNE` `BPL` `BVC` `BVS` |
| Status Flags | `CLC` `CLD` `CLI` `CLV` `SEC` `SED` `SEI` |

### Not yet implemented

`JMP` `JSR` `RTS` `BRK` `RTI` `NOP`

## Project Structure

```
emu6502/
├── include/            # Public headers
│   ├── common.h         # Shared typedefs (u8/u16/...) and bit-manipulation macros
│   ├── bus.h             # 64KB flat memory bus
│   ├── cpu_6502.h        # CPU state, opcode table, instruction/addressing-mode declarations
│   └── emulator.h        # Thin harness wiring the CPU to the bus
├── src/                # Implementation
│   ├── bus.cpp
│   ├── cpu_6502.cpp      # Opcode table + every instruction/addressing-mode implementation
│   └── emulator.cpp
├── emu/                # Example standalone executable
│   └── main.cpp
└── tests/               # Catch2 test suites, one per instruction family
    ├── tLoad.cpp
    ├── tStore.cpp
    ├── tTransfer.cpp
    ├── tStack.cpp
    ├── tLogical.cpp
    ├── tArithmetic.cpp
    ├── tIncDec.cpp
    ├── tShift.cpp
    ├── tBranch.cpp
    └── tStatusFlags.cpp
```

## Building

Requires CMake ≥ 3.16 and a C++17 compiler. Catch2 is fetched automatically via `FetchContent` on first configure.

```bash
cmake -B build
cmake --build build
```

This builds the `emu` executable and every test target defined in [tests/CMakeLists.txt](tests/CMakeLists.txt).

## Running the tests

Each instruction family is its own Catch2 executable and is registered with CTest:

```bash
cd build
ctest
```

Or run a single suite directly, e.g.:

```bash
./tests/Branch_Tests
```

## Architecture notes

- **`Bus`** is a flat `std::array<uint8_t, 64 * 1024>` — no memory mapping, mirroring, or I/O devices yet.
- **`CPU_6502`** exposes `clock()`, which advances the processor by a single cycle. On the cycle where the previous instruction's count reaches zero, it fetches the next opcode, runs its addressing mode and operation, and loads the total cycle count (base + any extra cycles from page-crossing or taken branches) for the next `clock()` calls to drain.
- **`Emulator`** wires a `CPU_6502` to a `Bus` and exposes `load_bytes_at_address`, `step`, and `execute(n)` for driving the CPU from tests or a host program.
