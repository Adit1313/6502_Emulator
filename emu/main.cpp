#include "main.h"
#include "cpu_6502.h"
#include "bus.h"

int main(int argc, char** argv)
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset.
    
    emu.reset(emu.RST_CPU);
    emu.load_bytes_at_address(0x301, std::vector<u8> {0x02});
    emu.load_bytes_at_address(0x200, std::vector<u8> {
        0xA2, 0x01,
        0xDE, 0x00, 0x03,
        0xBD, 0x00, 0x03
    });
    emu.execute(11);
    auto cpu = emu.get_CPU_obj();
    
    auto state = cpu.get_CPU_state();
    
    return 1;
}