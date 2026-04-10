#include "main.h"
#include "cpu_6502.h"
#include "bus.h"

int main(int argc, char** argv)
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x1, 0xA0, 0x8F, 0xAA});
    
    emu.reset(emu.RST_CPU);
    emu.execute(6);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    return 1;
}