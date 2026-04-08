#include "emu.h"

#include "cpu_6502.h"
#include "bus.h"

int main()
{
    // init the RAM
    Bus my_bus;

    my_bus.write(0xFFFD, 2);
    my_bus.write(0xFFFC, 0);

    my_bus.write(0x200, 0xA9);
    my_bus.write(0x201, 0x3);

    printf("%d", my_bus.read(0x200));

    // Innit CPU
    CPU_6502 my_cpu;
    my_cpu.connect_bus(&my_bus);
    my_cpu.reset();

    // run the CPU clock
    my_cpu.clock();
    my_cpu.clock();
    return 1;
}