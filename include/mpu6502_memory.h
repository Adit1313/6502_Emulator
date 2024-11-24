#ifndef MPU6502_MEMORY_HEADER
#define MPU6502_MEMORY_HEADER
#include <mpu6502_types.h>
#include <mpu6502_memory.h>

extern struct CPU cpu;

// Function with behaviour for memory
// initMemory - Rewrite the memory to 0s
void initMemory(Mem *memory);

BYTE FetchByte(CPU *cpu, Mem *memory);

BYTE ReadByte(Mem *memory, WORD address);

void WriteByte(CPU *cpu, Mem *memory, WORD address, BYTE value);

WORD FetchWord(CPU *cpu, Mem *memory);

WORD ReadWord(Mem *memory, WORD address);
#endif