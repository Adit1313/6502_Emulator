#include "main.h"
#include "cpu_6502.h"
#include "bus.h"

int main(int argc, char** argv)
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset.
    
    emu.reset(emu.RST_CPU);
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F, 0x08, 0xAC, 0xFD, 0x01});
    
    emu.execute(4);
    auto cpu = emu.get_CPU_obj();
    
    auto state = cpu.get_CPU_state();
    
    return 1;
}