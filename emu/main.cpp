#include "main.h"
#include "cpu_6502.h"
#include "bus.h"

int main(int argc, char** argv)
{
    Emulator emu;

    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x1, 0xB5, 0x20});
    emu.reset(emu.RST_CPU);

    emu.load_bytes_at_address(0x21, std::vector<u8> {0xA5});
    emu.execute(4);
    

    
    return 1;
}