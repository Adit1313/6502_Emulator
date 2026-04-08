#include "cpu_6502.h"

CPU_6502::CPU_6502()
{
    using CPU = CPU_6502;

    // Init opcode table with illegal opcode entries by default
    for (u8 idx = 0; idx < 0xFF; idx++)
    {
        opcode_table[idx] = {"XXX", &CPU::XXX, &CPU::IMM, 1};
    }
    // Populate valid opcodes
    opcode_table[0xA9] = {"LDA", &CPU::LDA, &CPU::IMM, 2};
}

CPU_6502::~CPU_6502()
{
    
}

void CPU_6502::reset()
{
    /*
    if cartridge reset routine
        go do cartridge reset routine
    else
    */
    // Standard cold start routine
    set_flag(I, false);
    SP = 0xFD;
    u16 low = read(0xFFFC);
    u16 high = read(0xFFFD);
    PC = (high << 8) | low;
}

u8 CPU_6502::read(u16 address)
{
    return 0; // For now until I create a bus or something
}

void CPU_6502::write(u16 address, u8 data)
{
    // Do nothing for now until a bus or memory is added
}

void CPU_6502::set_flag(FLAGS f, bool value)
{
    if (value)
        SET_BIT(flags, f);
    else
        CLEAR_BIT(flags, f);
}

bool CPU_6502::get_flag(FLAGS f)
{
    return GET_BIT(flags, f);
}

// Addressing Mode Definitions
u8 CPU_6502::IMM()
{
    
}