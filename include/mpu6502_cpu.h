#ifndef MPU6502_CPU_HEADER
#define MPU6502_CPU_HEADER
#include <mpu6502_types.h>

typedef union {
    BYTE byte;
    WORD word;
} OperandValue;

typedef enum {
    BYTE_OPERAND,
    WORD_OPERAND
} OperandType;

extern struct Mem memory;

extern void WriteStackToMemory(CPU *cpu, Mem *memory, BYTE value);
extern BYTE ReadStackFromMemory(CPU *cpu, Mem *memory);
extern void initMemory(Mem *memory);
extern BYTE FetchByte(CPU *cpu, Mem *memory);
extern BYTE ReadByte(Mem *memory, WORD address);
extern void WriteByte(CPU *cpu, Mem *memory, WORD address, BYTE value);
extern WORD FetchWord(CPU *cpu, Mem *memory);
extern WORD ReadWord(Mem *memory, WORD address);

// Defining opcodes
typedef OperandValue (*AddressingModeFcn)(CPU*, Mem*, OperandType*);
typedef void (*InstructionFcn)(CPU*, Mem*,  OperandValue, OperandType);

typedef struct {
    AddressingModeFcn addressModeFcn;
    InstructionFcn instrFcn;
} OpcodeEntry;

extern OpcodeEntry opcodeTable[256];

// Defining instructions
struct IntSet
{
    //LDA
    static constexpr BYTE LDA_IMMEDIATE = 0xA9;
    static constexpr BYTE LDA_ZEROPAGE = 0xA5;
    static constexpr BYTE LDA_ZEROPAGEADDX = 0xB5;
    static constexpr BYTE LDA_ABSOLUTE = 0xAD;
    static constexpr BYTE LDA_ABSOLUTEADDX = 0xBD;
    static constexpr BYTE LDA_ABSOLUTEADDY = 0xB9;
    static constexpr BYTE LDA_INDIRECTX = 0xA1;
    static constexpr BYTE LDA_INDIRECTY = 0xB1;
    // LDX 
    static constexpr BYTE LDX_IMMEDIATE = 0xA2;
    static constexpr BYTE LDX_ZEROPAGE = 0xA6;
    static constexpr BYTE LDX_ZEROPAGEADDX = 0xB6;
    static constexpr BYTE LDX_ABSOLUTE = 0xAE;
    static constexpr BYTE LDX_ABSOLUTEADDY = 0xBE;
    // LDY
    static constexpr BYTE LDY_IMMEDIATE = 0xA0;
    static constexpr BYTE LDY_ZEROPAGE = 0xA4;
    static constexpr BYTE LDY_ZEROPAGEADDX = 0xB4;
    static constexpr BYTE LDY_ABSOLUTE = 0xAC;
    static constexpr BYTE LDY_ABSOLUTEADDX = 0xBC;
    //STA
    static constexpr BYTE STA_ZEROPAGE = 0x85;
    static constexpr BYTE STA_ZEROPAGEADDX = 0x95;
    static constexpr BYTE STA_ABSOLUTE = 0x8D;
    static constexpr BYTE STA_ABSOLUTEADDX = 0x9D;
    static constexpr BYTE STA_ABSOLUTEADDY = 0x99;
    static constexpr BYTE STA_INDIRECTX = 0x81;
    static constexpr BYTE STA_INDIRECTY = 0x91;
    //STX
    static constexpr BYTE STX_ZEROPAGE = 0x86;
    static constexpr BYTE STX_ZEROPAGEADDY = 0x96;
    static constexpr BYTE STX_ABSOLUTE = 0x8E;
    //STY
    static constexpr BYTE STY_ZEROPAGE = 0x84;
    static constexpr BYTE STY_ZEROPAGEADDX = 0x94;
    static constexpr BYTE STY_ABSOLUTE = 0x8C;
    // JSR
    static constexpr BYTE JSR_ABSOLUTE = 0x20;
    //RTS
    static constexpr BYTE RTS = 0x60;
    // Invalid instruction
    static constexpr BYTE INVALID = 0x00;
};

// Functions to increment and decrement the stack

void incrementStackPointer(CPU *cpu);

void decrementStackPointer(CPU *cpu);

// Functions for different addressing modes

OperandValue impliedAddressingMode(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchImmediate(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchZeroPage(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchZeroPageAddX(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchAbsolute(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchAbsoluteAddX(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchAbsoluteAddY(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchIndirectAddX(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchIndirectAddY(CPU *cpu, Mem *mem, OperandType* opType);

// Addressing mode functions which return the address directly
OperandValue fetchZeroPageAddress(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchZeroPageAddXAddress(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchZeroPageAddYAddress(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchAbsoluteAddress(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchAbsoluteAddXAddress(CPU *cpu, Mem *mem, OperandType* opType);

OperandValue fetchAbsoluteAddYAddress(CPU *cpu, Mem *mem, OperandType* opType);

// Functions for different instructions
void LDA(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType);

void LDX(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType);

void LDY(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType);

void STA(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType);

void STX(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType);

void STY(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType);

void JSR(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType);

void RTS(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType);

void NULL_CMD(CPU* cpu, Mem *mem, OperandValue opVal, OperandType opType);

void InitOpcodeTable();

#endif