#include "cpu_6502.h"
#include "bus.h"

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

    addr_abs = 0;
    addr_rel = 0;
    mem_data = 0;
}

CPU_6502::~CPU_6502()
{
    
}

void CPU_6502::clock()
{
    if (current_cycles == 0)
    {
        u8 opcode = read(PC);

        // Fetch current instruction to be executed
        Instruction exec = opcode_table[opcode];
        // Check if any extra cycles were needed for execution (Ex: Page boundary crossed)
        u8 extra_cycles_1 = (this->*exec.addrmode)();
        u8 extra_cycles_2 = (this->*exec.operation)();

        current_cycles = exec.cycles + extra_cycles_1 + extra_cycles_2;
    }

    current_cycles -= 1;
}

void CPU_6502::reset()
{
    /*
    if cartridge reset routine
        go do cartridge reset routine
    else
    */
    // Standard cold start routine
    // set_flag(I, false); Do I need this?
    SP = STACK_POINTER_INIT;
    u16 low = read(0xFFFC);
    u16 high = read(0xFFFD);
    PC = (high << 8) | low;
    current_cycles = 0;
}

void CPU_6502::connect_bus(Bus *bus)
{
    bus_ptr = bus;
}

u8 CPU_6502::read(u16 address)
{
    return bus_ptr->read(address);
}

void CPU_6502::write(u16 address, u8 data)
{
    bus_ptr->write(address, data);
}

// Addressing Mode Definitions
#pragma region Addressing Mode Definitions
u8 CPU_6502::IMM()
{
    addr_abs = PC++;
    return 0;
}
#pragma endregion

// Operation Definitions
#pragma region Operation Definitions
u8 CPU_6502::LDA()
{
    fetch_mem();
    A = mem_data;
    return 0;
}

u8 CPU_6502::XXX()
{
    return 0;
}
#pragma endregion

#pragma region Helper Functions
void CPU_6502::fetch_mem()
{
    mem_data = read(addr_abs);
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
#pragma endregion