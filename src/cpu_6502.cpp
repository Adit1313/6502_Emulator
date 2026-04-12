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
    opcode_table[0x48] = {"PHA", &CPU::PHA, &CPU::IMP, 3};
    opcode_table[0x08] = {"PHP", &CPU::PHP, &CPU::IMP, 3};
    opcode_table[0x68] = {"PLA", &CPU::PLA, &CPU::IMP, 4};
    opcode_table[0x28] = {"PLP", &CPU::PLP, &CPU::IMP, 4};

    opcode_table[0x29] = {"AND", &CPU::AND, &CPU::IMM, 2};
    opcode_table[0x25] = {"AND", &CPU::AND, &CPU::ZP, 3};
    opcode_table[0x35] = {"AND", &CPU::AND, &CPU::ZPX, 4};
    opcode_table[0x2D] = {"AND", &CPU::AND, &CPU::ABS, 4};
    opcode_table[0x3D] = {"AND", &CPU::AND, &CPU::ABSX, 4};
    opcode_table[0x39] = {"AND", &CPU::AND, &CPU::ABSY, 4};
    opcode_table[0x21] = {"AND", &CPU::AND, &CPU::IZX, 6};
    opcode_table[0x31] = {"AND", &CPU::AND, &CPU::IZY, 5};

    opcode_table[0x49] = {"EOR", &CPU::EOR, &CPU::IMM, 2};
    opcode_table[0x45] = {"EOR", &CPU::EOR, &CPU::ZP, 3};
    opcode_table[0x55] = {"EOR", &CPU::EOR, &CPU::ZPX, 4};
    opcode_table[0x4D] = {"EOR", &CPU::EOR, &CPU::ABS, 4};
    opcode_table[0x5D] = {"EOR", &CPU::EOR, &CPU::ABSX, 4};
    opcode_table[0x59] = {"EOR", &CPU::EOR, &CPU::ABSY, 4};
    opcode_table[0x41] = {"EOR", &CPU::EOR, &CPU::IZX, 6};
    opcode_table[0x51] = {"EOR", &CPU::EOR, &CPU::IZY, 5};

    opcode_table[0x09] = {"ORA", &CPU::ORA, &CPU::IMM, 2};
    opcode_table[0x05] = {"ORA", &CPU::ORA, &CPU::ZP, 3};
    opcode_table[0x15] = {"ORA", &CPU::ORA, &CPU::ZPX, 4};
    opcode_table[0x0D] = {"ORA", &CPU::ORA, &CPU::ABS, 4};
    opcode_table[0x1D] = {"ORA", &CPU::ORA, &CPU::ABSX, 4};
    opcode_table[0x19] = {"ORA", &CPU::ORA, &CPU::ABSY, 4};
    opcode_table[0x01] = {"ORA", &CPU::ORA, &CPU::IZX, 6};
    opcode_table[0x11] = {"ORA", &CPU::ORA, &CPU::IZY, 5};

    opcode_table[0x24] = {"BIT", &CPU::BIT, &CPU::ZP, 3};
    opcode_table[0x2C] = {"BIT", &CPU::BIT, &CPU::ABS, 4};

    opcode_table[0x69] = {"ADC", &CPU_6502::ADC, &CPU_6502::IMM, 2};
    opcode_table[0x65] = {"ADC", &CPU_6502::ADC, &CPU_6502::ZP, 3};
    opcode_table[0x75] = {"ADC", &CPU_6502::ADC, &CPU_6502::ZPX, 4};
    opcode_table[0x6D] = {"ADC", &CPU_6502::ADC, &CPU_6502::ABS, 4};
    opcode_table[0x7D] = {"ADC", &CPU_6502::ADC, &CPU_6502::ABSX, 4};
    opcode_table[0x79] = {"ADC", &CPU_6502::ADC, &CPU_6502::ABSY, 4};
    opcode_table[0x61] = {"ADC", &CPU_6502::ADC, &CPU_6502::IZX, 6};
    opcode_table[0x71] = {"ADC", &CPU_6502::ADC, &CPU_6502::IZY, 5};

    opcode_table[0xE9] = {"SBC", &CPU_6502::SBC, &CPU_6502::IMM, 2};
    opcode_table[0xE5] = {"SBC", &CPU_6502::SBC, &CPU_6502::ZP, 3};
    opcode_table[0xF5] = {"SBC", &CPU_6502::SBC, &CPU_6502::ZPX, 4};
    opcode_table[0xED] = {"SBC", &CPU_6502::SBC, &CPU_6502::ABS, 4};
    opcode_table[0xFD] = {"SBC", &CPU_6502::SBC, &CPU_6502::ABSX, 4};
    opcode_table[0xF9] = {"SBC", &CPU_6502::SBC, &CPU_6502::ABSY, 4};
    opcode_table[0xE1] = {"SBC", &CPU_6502::SBC, &CPU_6502::IZX, 6};
    opcode_table[0xF1] = {"SBC", &CPU_6502::SBC, &CPU_6502::IZY, 5};

    opcode_table[0xC9] = {"CMP", &CPU_6502::CMP, &CPU_6502::IMM, 2};
    opcode_table[0xC5] = {"CMP", &CPU_6502::CMP, &CPU_6502::ZP, 3};
    opcode_table[0xD5] = {"CMP", &CPU_6502::CMP, &CPU_6502::ZPX, 4};
    opcode_table[0xCD] = {"CMP", &CPU_6502::CMP, &CPU_6502::ABS, 4};
    opcode_table[0xDD] = {"CMP", &CPU_6502::CMP, &CPU_6502::ABSX, 4};
    opcode_table[0xD9] = {"CMP", &CPU_6502::CMP, &CPU_6502::ABSY, 4};
    opcode_table[0xC1] = {"CMP", &CPU_6502::CMP, &CPU_6502::IZX, 6};
    opcode_table[0xD1] = {"CMP", &CPU_6502::CMP, &CPU_6502::IZY, 5};

    opcode_table[0xE0] = {"CPX", &CPU_6502::CPX, &CPU_6502::IMM, 2};
    opcode_table[0xE4] = {"CPX", &CPU_6502::CPX, &CPU_6502::ZP, 3};
    opcode_table[0xEC] = {"CPX", &CPU_6502::CPX, &CPU_6502::ABS, 4};

    opcode_table[0xC0] = {"CPY", &CPU_6502::CPY, &CPU_6502::IMM, 2};
    opcode_table[0xC4] = {"CPY", &CPU_6502::CPY, &CPU_6502::ZP, 3};
    opcode_table[0xCC] = {"CPY", &CPU_6502::CPY, &CPU_6502::ABS, 4};
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
    addr_abs |= read(PC++) << 8;
    return 0;
}

u8 CPU_6502::ABSX()
{
    u16 base = read(PC++);
    base |=  read(PC++) << 8;
    addr_abs = base + X;

    return (addr_abs & 0xFF00) != (base & 0xFF00);
}

u8 CPU_6502::ABSY()
{
    u16 base = read(PC++);
    base |= read(PC++) << 8;
    addr_abs = base + Y;

    return (addr_abs & 0xFF00) != (base & 0xFF00);
}

u8 CPU_6502::IZX()
{
    u8 zp_base = read(PC++);
    zp_base = (X + zp_base)%0x100;
    addr_abs = read(zp_base) | read(zp_base+1) << 8;
    return 0;
}

u8 CPU_6502::IZY()
{
    u8 zp_base = read(PC++);
    u16 base = read(zp_base) | read(zp_base+1) << 8;
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

u8 CPU_6502::AND()
{
    fetch_mem();
    A = A & mem_data;
    update_zn_flags(A);
    return 0;
}

u8 CPU_6502::EOR()
{
    fetch_mem();
    A = A ^ mem_data;
    update_zn_flags(A);
    return 0;
}

u8 CPU_6502::ORA()
{
    fetch_mem();
    A = A | mem_data;
    update_zn_flags(A);
    return 0;
}

u8 CPU_6502::BIT()
{
    fetch_mem();

    if (!(A&mem_data))
        SET_BIT(flags, Z);
    else
        CLEAR_BIT(flags, Z);

    if (GET_BIT(mem_data, 6))
        SET_BIT(flags, V);
    else
        CLEAR_BIT(flags, V);

    if (GET_BIT(mem_data, 7))
        SET_BIT(flags, N);
    else
        CLEAR_BIT(flags, N);
    
    return 0;
}

u8 CPU_6502::ADC()
{
    fetch_mem();
    u16 sum = (u16)A + (u16)mem_data + (u16)GET_BIT(flags, C);

    // Carry flag
    if (sum > 0xFF)
        SET_BIT(flags, C);
    else
        CLEAR_BIT(flags, C);

    u8 result = sum & 0xFF;

    // Overflow flag
    if ((~(A ^ mem_data) & (A ^ result)) & 0x80)
        SET_BIT(flags, V);
    else
        CLEAR_BIT(flags, V);

    A = result;
    update_zn_flags(A);
    return 0;
}

u8 CPU_6502::SBC()
{
    fetch_mem();
    u8 value = mem_data ^ 0xFF;
    u16 sum = (u16)A + (u16)value + (u16)GET_BIT(flags, C);

    // Carry flag (same logic as ADC)
    if (sum > 0xFF)
        SET_BIT(flags, C);
    else
        CLEAR_BIT(flags, C);

    u8 result = sum & 0xFF;

    // Overflow flag
    if ((~(A ^ value) & (A ^ result)) & 0x80)
        SET_BIT(flags, V);
    else
        CLEAR_BIT(flags, V);

    A = result;
    update_zn_flags(A);
    return 0;
}

u8 CPU_6502::CMP()
{
    fetch_mem();
    u16 temp = (u16)A - (u16)mem_data;

    // Carry flag (A >= M)
    if (A >= mem_data)
        SET_BIT(flags, C);
    else
        CLEAR_BIT(flags, C);

    // Z and N from result
    update_zn_flags((u8)temp);

    return 0;
}

u8 CPU_6502::CPX()
{
    fetch_mem();

    u16 temp = (u16)X - (u16)mem_data;

    if (X >= mem_data)
        SET_BIT(flags, C);
    else
        CLEAR_BIT(flags, C);

    update_zn_flags((u8)temp);

    return 0;
}

u8 CPU_6502::CPY()
{
    fetch_mem();

    u16 temp = (u16)Y - (u16)mem_data;

    if (Y >= mem_data)
        SET_BIT(flags, C);
    else
        CLEAR_BIT(flags, C);

    update_zn_flags((u8)temp);

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