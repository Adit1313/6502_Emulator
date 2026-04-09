#include "emulator.h"
#include <cassert>
#include <cstdio>

void test_LDA_IMM()
{
    printf("Testing LDA - IMM addressing");
    Emulator emu;

    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F, 0xA9, 0x2, 0xA9, 0x0});
    emu.reset(emu.RST_CPU);

    u8 limit = 2;
    while (limit > 0)
    {
        emu.step();
        limit -= 1;
    }

    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_State();

    printf("LDA - IMM: Verify Accumulator Value");
    assert(state.A == 0x8F);
    printf("LDA - IMM: Verify N Flag Set");
    assert(cpu.get_flag(CPU_6502::N) == 1);

    limit = 2;
    while (limit > 0)
    {
        emu.step();
        limit -= 1;
    }

    cpu = emu.get_CPU_obj();
    printf("LDA - IMM: Verify N Flag Not Set");
    assert(cpu.get_flag(CPU_6502::N) == 0);

    limit = 2;
    while (limit > 0)
    {
        emu.step();
        limit -= 1;
    }

    cpu = emu.get_CPU_obj();
    printf("LDA - IMM: Verify Z Flag Set");
    assert(cpu.get_flag(CPU_6502::Z) == 1);
}

void test_LDA_ZP()
{
    printf("Testing LDA - ZP addressing");
    Emulator emu;

    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F, 0xA9, 0x2, 0xA9, 0x0});
    emu.reset(emu.RST_CPU);

    u8 limit = 2;
    while (limit > 0)
    {
        emu.step();
        limit -= 1;
    }

    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_State();

    printf("LDA - ZP: Verify Accumulator Value");
    assert(state.A == 0x8F);
    printf("LDA - ZP: Verify N Flag Set");
    assert(cpu.get_flag(CPU_6502::N) == 1);

    limit = 2;
    while (limit > 0)
    {
        emu.step();
        limit -= 1;
    }

    cpu = emu.get_CPU_obj();
    printf("LDA - ZP: Verify N Flag Not Set");
    assert(cpu.get_flag(CPU_6502::N) == 0);

    limit = 2;
    while (limit > 0)
    {
        emu.step();
        limit -= 1;
    }

    cpu = emu.get_CPU_obj();
    printf("LDA - ZP: Verify Z Flag Set");
    assert(cpu.get_flag(CPU_6502::Z) == 1);
}

int main()
{
    test_LDA_IMM();
    test_LDA_ZP();

    return 0;
}