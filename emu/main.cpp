#include "main.h"
#include "cpu_6502.h"
#include "bus.h"

int main(int argc, char** argv)
{
    // init the RAM
    Emulator emu;

    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0});
    emu.load_bytes_at_address(0x20, std::vector<u8> {129});
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0x8F, 0xA6, 0x20});

    emu.reset(emu.RST_CPU);

    // Bus my_bus;

    // my_bus.write(0xFFFD, 2);
    // my_bus.write(0xFFFC, 0);
    // my_bus.write(0x20, 129);

    // my_bus.write(0x200, 0xA9);
    // my_bus.write(0x201, 0x8F);
    // my_bus.write(0x202, 0xA6);
    // my_bus.write(0x203, 0x20);

    // // Innit CPU
    // CPU_6502 my_cpu;
    // my_cpu.connect_bus(&my_bus);
    // my_cpu.reset();

    // // run the CPU clock
    u8 limit = 5;
    while (limit > 0)
    {
        emu.step();
        limit -= 1;
    }
    
    return 1;
}