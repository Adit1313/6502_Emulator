#include <mpu6502_cpu.h>

u32 cycles = 0;
OpcodeEntry opcodeTable[256];

// Functions for different addressing modes
OperandValue fetchImmediate(CPU *cpu, Mem *mem, OperandType* opType) 
{
    *opType = BYTE_OPERAND;
    OperandValue value = {0};
    value.byte = FetchByte(cpu, mem);
    return value;
}

OperandValue fetchZeroPage(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = BYTE_OPERAND;
    OperandValue value = {0};
    BYTE zpAddr = FetchByte(cpu, mem);
    value.byte = ReadByte(mem, zpAddr);
    return value;
}

OperandValue fetchZeroPageAddX(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = BYTE_OPERAND;
    OperandValue value = {0};
    BYTE zpAddr = FetchByte(cpu, mem);
    
    zpAddr += cpu->X;   // Add the value of X to the address
    cycles += 1;        // Requires an extra cycle

    value.byte = ReadByte(mem, zpAddr);
    return value;
}

OperandValue fetchAbsolute(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = BYTE_OPERAND;
    OperandValue value = {0};
    WORD addr = FetchWord(cpu, mem);
    value.byte = ReadByte(mem, addr);
    return value;
}

OperandValue fetchAbsoluteAddX(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = BYTE_OPERAND;
    OperandValue value = {0};
    WORD addr = FetchWord(cpu, mem);

    if ((addr + cpu->X) >> 8 != (addr >> 8)) // Detect a page crossing
    {
        cycles += 1;
    }
    addr += cpu->X;     // Add the value of X to the address

    value.byte = ReadByte(mem, addr);
    return value;
}

OperandValue fetchAbsoluteAddY(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = BYTE_OPERAND;
    OperandValue value = {0};
    WORD addr = FetchWord(cpu, mem);

    if ((addr + cpu->Y) >> 8 != (addr >> 8)) // Detect a page crossing
    {
        cycles += 1;
    }
    addr += cpu->Y;     // Add the value of X to the address

    value.byte = ReadByte(mem, addr);
    return value;
}

OperandValue fetchIndirectAddX(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = BYTE_OPERAND;
    OperandValue value = {0};
    BYTE zeroPageAddress = FetchByte(cpu, mem);
    zeroPageAddress += cpu->X;
    cycles += 1;
    WORD vector = ReadWord(mem, zeroPageAddress);
    value.byte = ReadByte(mem, vector);
    return value;
}

OperandValue fetchIndirectAddY(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = BYTE_OPERAND;
    OperandValue value = {0};

    BYTE zeroPageAddress = FetchByte(cpu, mem);
    WORD vector = ReadWord(mem, zeroPageAddress);

    if ((vector + cpu->Y) >> 8 != (vector >> 8)) // Detect a page crossing
    {
        cycles += 1;
    }

    vector += cpu->Y;
    value.byte = ReadByte(mem, vector);
    return value;
}

OperandValue fetchZeroPageAddress(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = WORD_OPERAND;
    OperandValue value = {0};
    value.word = FetchByte(cpu, mem);
    return value;
}

OperandValue fetchZeroPageAddXAddress(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = WORD_OPERAND;
    OperandValue value = {0};
    value.word = FetchByte(cpu, mem);
    
    value.word += cpu->X;   // Add the value of X to the address
    cycles += 1;        // Requires an extra cycle

    return value;
}

OperandValue fetchZeroPageAddYAddress(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = WORD_OPERAND;
    OperandValue value = {0};
    value.word = FetchByte(cpu, mem);
    
    value.word += cpu->Y;   // Add the value of X to the address
    cycles += 1;        // Requires an extra cycle

    return value;
}

OperandValue fetchAbsoluteAddress(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = WORD_OPERAND;
    OperandValue value = {0};
    value.word = FetchWord(cpu, mem);
    return value;
}

OperandValue fetchAbsoluteAddXAddress(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = WORD_OPERAND;
    OperandValue value = {0};
    value.word = FetchWord(cpu, mem);

    value.word += cpu->X;
    cycles += 1;

    return value;
}

OperandValue fetchAbsoluteAddYAddress(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = WORD_OPERAND;
    OperandValue value = {0};
    value.word = FetchWord(cpu, mem);

    value.word += cpu->Y;
    cycles += 1;

    return value;
}

OperandValue fetchIndirectAddXAddress(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = WORD_OPERAND;
    OperandValue value = {0};
    BYTE zeroPageAddress = FetchByte(cpu, mem);
    zeroPageAddress += cpu->X;
    cycles += 1;
    WORD vector = ReadWord(mem, zeroPageAddress);
    value.word = vector;
    return value;
}

OperandValue fetchIndirectAddYAddress(CPU *cpu, Mem *mem, OperandType* opType)
{
    *opType = WORD_OPERAND;
    OperandValue value = {0};

    BYTE zeroPageAddress = FetchByte(cpu, mem);
    WORD vector = ReadWord(mem, zeroPageAddress);
    vector += cpu->Y;
    cycles += 1;
    value.word = vector;
    return value;
}

// Functions for different instructions
void LDA(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType)
{
    cpu->A = opVal.byte;
    cpu->Z = (cpu->A == 0);
    cpu->N = cpu->A&0b10000000 > 0;
}

void LDX(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType)
{
    cpu->X = opVal.byte;
    cpu->Z = (cpu->X == 0);
    cpu->N = cpu->X&0b10000000 > 0;
}

void LDY(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType)
{
    cpu->Y = opVal.byte;
    cpu->Z = (cpu->Y == 0);
    cpu->N = cpu->Y&0b10000000 > 0;
}

void STA(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType)
{
    WriteByte(cpu, mem, opVal.word, cpu->A);
}

void STX(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType)
{
    WriteByte(cpu, mem, opVal.word, cpu->X);
}

void STY(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType)
{
    WriteByte(cpu, mem, opVal.word, cpu->Y);
}

void JSR(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType)
{
    (*mem)[cpu->SP] = (cpu->PC) >> 8;
    cpu->SP -= 1;
    cycles += 1;
    (*mem)[cpu->SP] = cpu->PC;
    cpu->SP -= 1;
    cycles += 1;
    cpu->PC = opVal.word;
    cycles += 1;
}

void NULL_CMD(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType)
{
    // error case
}

void InitOpcodeTable()
{
    opcodeTable[IntSet::LDA_IMMEDIATE] = (OpcodeEntry){fetchImmediate, LDA};
    opcodeTable[IntSet::LDA_ZEROPAGE] = (OpcodeEntry){fetchZeroPage, LDA};
    opcodeTable[IntSet::LDA_ZEROPAGEADDX] = (OpcodeEntry){fetchZeroPageAddX, LDA};
    opcodeTable[IntSet::LDA_ABSOLUTE] = (OpcodeEntry){fetchAbsolute, LDA};
    opcodeTable[IntSet::LDA_ABSOLUTEADDX] = (OpcodeEntry){fetchAbsoluteAddX, LDA};
    opcodeTable[IntSet::LDA_ABSOLUTEADDY] = (OpcodeEntry){fetchAbsoluteAddY, LDA};
    opcodeTable[IntSet::LDA_INDIRECTX] = (OpcodeEntry){fetchIndirectAddX, LDA};
    opcodeTable[IntSet::LDA_INDIRECTY] = (OpcodeEntry){fetchIndirectAddY, LDA};

    opcodeTable[IntSet::LDX_IMMEDIATE] = (OpcodeEntry){fetchImmediate, LDX};
    opcodeTable[IntSet::LDX_ZEROPAGE] = (OpcodeEntry){fetchZeroPage, LDX};
    opcodeTable[IntSet::LDX_ZEROPAGEADDX] = (OpcodeEntry){fetchZeroPageAddX, LDX};
    opcodeTable[IntSet::LDX_ABSOLUTE] = (OpcodeEntry){fetchAbsolute, LDX};
    opcodeTable[IntSet::LDX_ABSOLUTEADDY] = (OpcodeEntry){fetchAbsoluteAddY, LDX};

    opcodeTable[IntSet::LDY_IMMEDIATE] = (OpcodeEntry){fetchImmediate, LDY};
    opcodeTable[IntSet::LDY_ZEROPAGE] = (OpcodeEntry){fetchZeroPage, LDY};
    opcodeTable[IntSet::LDY_ZEROPAGEADDX] = (OpcodeEntry){fetchZeroPageAddX, LDY};
    opcodeTable[IntSet::LDY_ABSOLUTE] = (OpcodeEntry){fetchAbsolute, LDY};
    opcodeTable[IntSet::LDY_ABSOLUTEADDX] = (OpcodeEntry){fetchAbsoluteAddY, LDY};

    opcodeTable[IntSet::STA_ZEROPAGE] = (OpcodeEntry){fetchZeroPageAddress, STA};
    opcodeTable[IntSet::STA_ZEROPAGEADDX] = (OpcodeEntry){fetchZeroPageAddXAddress, STA};
    opcodeTable[IntSet::STA_ABSOLUTE] = (OpcodeEntry){fetchAbsoluteAddress, STA};
    opcodeTable[IntSet::STA_ABSOLUTEADDX] = (OpcodeEntry){fetchAbsoluteAddXAddress, STA};
    opcodeTable[IntSet::STA_ABSOLUTEADDY] = (OpcodeEntry){fetchAbsoluteAddYAddress, STA};
    opcodeTable[IntSet::STA_INDIRECTX] = (OpcodeEntry){fetchIndirectAddXAddress, STA};
    opcodeTable[IntSet::STA_INDIRECTY] = (OpcodeEntry){fetchIndirectAddYAddress, STA};

    opcodeTable[IntSet::STX_ZEROPAGE] = (OpcodeEntry){fetchZeroPageAddress, STX};
    opcodeTable[IntSet::STX_ZEROPAGEADDY] = (OpcodeEntry){fetchZeroPageAddYAddress, STX};
    opcodeTable[IntSet::STX_ABSOLUTE] = (OpcodeEntry){fetchAbsoluteAddress, STX};

    opcodeTable[IntSet::STY_ZEROPAGE] = (OpcodeEntry){fetchZeroPageAddress, STY};
    opcodeTable[IntSet::STY_ZEROPAGEADDX] = (OpcodeEntry){fetchZeroPageAddXAddress, STY};
    opcodeTable[IntSet::STY_ABSOLUTE] = (OpcodeEntry){fetchAbsoluteAddress, STY};

    opcodeTable[IntSet::JSR_ABSOLUTE] = (OpcodeEntry){fetchAbsoluteAddress, JSR};
    opcodeTable[IntSet::INVALID] = (OpcodeEntry){fetchImmediate, NULL_CMD};
}