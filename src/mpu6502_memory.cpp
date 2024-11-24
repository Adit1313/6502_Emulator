#include <mpu6502_memory.h>
#include <mpu6502_cpu.h>

void initMemory(Mem *memory)
{
    for (u32 i = 0; i < memory->MAX_MEM; i++)
    {
        memory->Data[i] = 0;
    }
}

BYTE FetchByte(CPU *cpu, Mem *memory)
{
    BYTE data = (*memory)[cpu->PC];
    cpu->PC += 1;
    cycles += 1;
    return data;
}

BYTE ReadByte(Mem *memory, WORD address)
{
    BYTE data = (*memory)[address];
    cycles += 1;
    return data;
}

void WriteByte(CPU *cpu, Mem *memory, WORD address, BYTE value)
{
    (*memory)[address] = value;
    cycles += 1;
    cpu->PC += 1;
}

WORD FetchWord(CPU *cpu, Mem *memory)
{
    WORD data = ((*memory)[cpu->PC]) | ((*memory)[cpu->PC+1] << 8);
    cpu->PC += 2;
    cycles += 2;
    return data;
}

WORD ReadWord(Mem *memory, WORD address)
{
    WORD data = ((*memory)[address]) | ((*memory)[address+1] << 8);
    cycles += 2;
    return data;
}