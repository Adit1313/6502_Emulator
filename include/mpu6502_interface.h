#ifndef MPU6502_INTERFACE_HEADER
#define MPU6502_INTERFACE_HEADER

#include <mpu6502_types.h>
#include <mpu6502_cpu.h>
#include <mpu6502_memory.h>

//Functions with behaviour for CPU
// Reset - Reset all CPU registers. Initialize the memory.
void Reset(CPU *cpu, Mem *memory);

// Execute - Runs the CPU for a given number of cycles
void Execute(CPU *cpu, Mem *memory, u32 instructionLimit);

#endif