#include "main.h"
#include "cpu_6502.h"
#include "bus.h"

int main(int argc, char** argv)
{
    // Emulator emu;

    // emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0});
    // emu.load_bytes_at_address(0x20, std::vector<u8> {129});
    // emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F, 0xA6, 0x20});

    // emu.reset(emu.RST_CPU);

    // // // run the CPU clock
    // u8 limit = 5;
    // while (limit > 0)
    // {
    //     emu.step();
    //     limit -= 1;
    // }

    // printf("Testing LDA - IMM addressing");
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

    // printf("LDA - IMM: Verify Accumulator Value");
    // assert(state.A == 0x8F);
    // printf("LDA - IMM: Verify N Flag Set");
    // assert(cpu.get_flag(cpu.N) == 0x8F);

    limit = 2;
    while (limit > 0)
    {
        emu.step();
        limit -= 1;
    }

    // printf("LDA - IMM: Verify N Flag Not Set");
    // assert(cpu.get_flag(cpu.N) == 0);

    limit = 2;
    while (limit > 0)
    {
        emu.step();
        limit -= 1;
    }

    // printf("LDA - IMM: Verify Z Flag Set");
    // assert(cpu.get_flag(cpu.Z) == 1);
    
    return 1;
}