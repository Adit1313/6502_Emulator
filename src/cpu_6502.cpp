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
    
    #pragma region Opcode Definition

    opcode_table[0xA9] = {"LDA", &CPU::LDA, &CPU::IMM, 2};
    opcode_table[0xA5] = {"LDA", &CPU::LDA, &CPU::ZP, 3};
    opcode_table[0xB5] = {"LDA", &CPU::LDA, &CPU::ZPX, 4};
    opcode_table[0xAD] = {"LDA", &CPU::LDA, &CPU::ABS, 4};
    opcode_table[0xBD] = {"LDA", &CPU::LDA, &CPU::ABSX, 4};
    opcode_table[0xB9] = {"LDA", &CPU::LDA, &CPU::ABSY, 4};
    opcode_table[0xA1] = {"LDA", &CPU::LDA, &CPU::IZX, 6};
    opcode_table[0xB1] = {"LDA", &CPU::LDA, &CPU::IZY, 5};

    opcode_table[0xA2] = {"LDX", &CPU::LDX, &CPU::IMM, 2};
    opcode_table[0xA6] = {"LDX", &CPU::LDX, &CPU::ZP, 3};
    opcode_table[0xB6] = {"LDX", &CPU::LDX, &CPU::ZPY, 4};
    opcode_table[0xAE] = {"LDX", &CPU::LDX, &CPU::ABS, 4};
    opcode_table[0xBE] = {"LDX", &CPU::LDX, &CPU::ABSY, 4};

    opcode_table[0xA0] = {"LDY", &CPU::LDY, &CPU::IMM, 2};
    opcode_table[0xA4] = {"LDY", &CPU::LDY, &CPU::ZP, 3};
    opcode_table[0xB4] = {"LDY", &CPU::LDY, &CPU::ZPX, 4};
    opcode_table[0xAC] = {"LDY", &CPU::LDY, &CPU::ABS, 4};
    opcode_table[0xBC] = {"LDY", &CPU::LDY, &CPU::ABSX, 4};

    opcode_table[0x85] = {"STA", &CPU::STA, &CPU::ZP, 3};
    opcode_table[0x95] = {"STA", &CPU::STA, &CPU::ZPX, 4};
    opcode_table[0x8D] = {"STA", &CPU::STA, &CPU::ABS, 4};
    opcode_table[0x9D] = {"STA", &CPU::STA, &CPU::ABSX, 5};
    opcode_table[0x99] = {"STA", &CPU::STA, &CPU::ABSY, 5};
    opcode_table[0x81] = {"STA", &CPU::STA, &CPU::IZX, 6};
    opcode_table[0x91] = {"STA", &CPU::STA, &CPU::IZY, 6};

    opcode_table[0x86] = {"STX", &CPU::STX, &CPU::ZP, 3};
    opcode_table[0x96] = {"STX", &CPU::STX, &CPU::ZPY, 4};
    opcode_table[0x8E] = {"STX", &CPU::STX, &CPU::ABS, 4};

    opcode_table[0x84] = {"STY", &CPU::STY, &CPU::ZP, 3};
    opcode_table[0x94] = {"STY", &CPU::STY, &CPU::ZPX, 4};
    opcode_table[0x8C] = {"STY", &CPU::STY, &CPU::ABS, 4};

    opcode_table[0xAA] = {"TAX", &CPU::TAX, &CPU::IMP, 2};
    opcode_table[0xA8] = {"TAY", &CPU::TAY, &CPU::IMP, 2};
    opcode_table[0x8A] = {"TXA", &CPU::TXA, &CPU::IMP, 2};
    opcode_table[0x98] = {"TYA", &CPU::TYA, &CPU::IMP, 2};

    opcode_table[0xBA] = {"TSX", &CPU::TSX, &CPU::IMP, 2};
    opcode_table[0x9A] = {"TXS", &CPU::TXS, &CPU::IMP, 2};
    opcode_table[0x49] = {"PHA", &CPU::PHA, &CPU::IMP, 3};
    opcode_table[0x08] = {"PHP", &CPU::PHP, &CPU::IMP, 3};
    opcode_table[0x68] = {"PLA", &CPU::PLA, &CPU::IMP, 4};
    opcode_table[0x28] = {"PLP", &CPU::PLP, &CPU::IMP, 4};


    #pragma endregion

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

        PC++;

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
u8 CPU_6502::IMP()
{
    return 0;
}

u8 CPU_6502::IMM()
{
    addr_abs = PC++;
    return 0;
}

u8 CPU_6502::ZP()
{
    addr_abs = read(PC++);
    return 0;
}

u8 CPU_6502::ZPX()
{
    addr_abs = read(PC++)+X;
    return 0;
}

u8 CPU_6502::ZPY()
{
    addr_abs = read(PC++)+Y;
    return 0;
}

u8 CPU_6502::ABS()
{
    addr_abs = read(PC++);
    addr_abs = (addr_abs << 8) | read(PC++);
    return 0;
}

u8 CPU_6502::ABSX()
{
    u16 base = read(PC++);
    base = (base << 8) | read(PC++);
    addr_abs = base + X;

    return (addr_abs & 0xFF00) != (base & 0xFF00);
}

u8 CPU_6502::ABSY()
{
    u16 base = read(PC++);
    base = (base << 8) | read(PC++);
    addr_abs = base + Y;

    return (addr_abs & 0xFF00) != (base & 0xFF00);
}

u8 CPU_6502::IZX()
{
    u8 zp_base = read(PC++);
    zp_base = (X + zp_base)%0x100;
    addr_abs = read(zp_base)<<8 | read(zp_base+1);
    return 0;
}

u8 CPU_6502::IZY()
{
    u8 zp_base = read(PC++);
    u16 base = read(zp_base) << 8 | read(zp_base+1);
    addr_abs = base + Y;
    return (addr_abs & 0xFF00) != (base & 0xFF00);
}
#pragma endregion

// Operation Definitions
#pragma region Operation Definitions
u8 CPU_6502::LDA()
{
    fetch_mem();
    A = mem_data;

    update_zn_flags(A);
    return 0;
}

u8 CPU_6502::LDX()
{
    fetch_mem();
    X = mem_data;
    update_zn_flags(X);
    return 0;
}

u8 CPU_6502::LDY()
{
    fetch_mem();
    Y = mem_data;
    update_zn_flags(Y);
    return 0;
}

u8 CPU_6502::STA()
{
    write(addr_abs, A);
    return 0;
}

u8 CPU_6502::STX()
{
    write(addr_abs, X);
    return 0;
}

u8 CPU_6502::STY()
{
    write(addr_abs, Y);
    return 0;
}

u8 CPU_6502::TAX()
{
    X = A;
    update_zn_flags(X);
    return 0;
}

u8 CPU_6502::TAY()
{
    Y = A;
    update_zn_flags(Y);
    return 0;
}

u8 CPU_6502::TXA()
{
    A = X;
    update_zn_flags(A);
    return 0;
}

u8 CPU_6502::TYA()
{
    A = Y;
    update_zn_flags(A);
    return 0;
}

u8 CPU_6502::TSX()
{
    X = SP;
    update_zn_flags(X);
    return 0;
}

u8 CPU_6502::TXS()
{
    SP = X;
    return 0;
}

u8 CPU_6502::PHA()
{
    write(0x100 + SP--, A);
    return 0;
}

u8 CPU_6502::PHP()
{
    write(0x100 + SP--, flags);
    return 0;
}

u8 CPU_6502::PLA()
{
    A = read(0x100 + ++SP);
    return 0;
}

u8 CPU_6502::PLP()
{
    flags = read(0x100 + ++SP);
    return 0;
}

u8 CPU_6502::XXX()
{
    return 0; // idk what to do here
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

CPU_State CPU_6502::get_CPU_state()
{
    return {PC, SP, A, X, Y, flags};
}

inline void CPU_6502::update_zn_flags(u8 reg)
{
    if (reg == 0)
        SET_BIT(flags, Z);
    else
        CLEAR_BIT(flags, Z);

    if (GET_BIT(reg, 7))
        SET_BIT(flags, N);
    else
        CLEAR_BIT(flags, N);
}
#pragma endregion