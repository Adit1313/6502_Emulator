
#include <mpu6502_interface.h>

int main(void)
{
    // Initilise our CPU and memory
    CPU cpu;
    Mem memory;
    InitOpcodeTable();
    Reset(&cpu, &memory);

    // Temp hardcoding of memory for testing
    memory[0xFF00] = IntSet::LDA_ABSOLUTEADDY;
    memory[0xFF01] = 0x44;
    memory[0xFF02] = 0x02;
    memory[0xFFFC] = 0x01;
    // memory[0x4403] = 0x29;
    memory[0x4501] = 0x29;
    cpu.Y = 0xFF;

    // Begin program execution
    Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    
    return 0;
}