#pragma once

// ------ Includes ------
#include "common.h"

// ------ Defines ------
#define RESET_VECTOR 0xFFFC // Points to the system reset routine
#define STACK_POINTER_INIT 0xFD

/*
CPU Contents:
1. Program Counter  - 16 bits
2. Stack Pointer - 8 bits
3. Accumulator - 8 bits
4. Register X - 8 bits
5. Register Y - 8 bits
6. Flags - 1 bit each, 8 bits total
    Carry
    Zero
    Interrupt Disable
    Decimal Mode
    Break Command
    Overflow Flag
    Negative Flag 
*/

/*
Boot behaviour:
The reset vector tells the CPU where to find the system reset routine.
The address of this routine is stored in low byte then high byte order.
*/

struct CPU_State
{
    u16 PC;
    u8 SP, A, X, Y, flags;
};

class Bus;

class CPU_6502 {
    public:
        CPU_6502();
        ~CPU_6502();

        enum FLAGS {
            C = 0, // Carry Flag - Set if the last operation caused an overflow from bit 7 of the result or an underflow from bit 0.
            Z = 1, // Zero Flag  - The zero flag is set if the result of the last operation as was zero.
            I = 2, // Interrupt Disable - While this flag is set the processor will not respond to interrupts from devices until it is cleared.
            D = 3, // Decimal Mode - While the decimal mode flag is set the processor will obey the rules of Binary Coded Decimal (BCD) arithmetic during addition and subtraction.
            B = 4, // Break Command - The break command bit is set when a BRK instruction has been executed and an interrupt has been generated to process it.
            V = 5, // Overflow Flag - The overflow flag is set during arithmetic operations if the result has yielded an invalid 2's complement result.
            N = 6  // Negative Flag - The negative flag is set if the result of the last operation had bit 7 set to a one.
        };

        void clock();
        void reset();

        // Bus Access Functions
        void connect_bus(Bus *bus);
        void write(u16 address, u8 data);
        u8 read(u16 address);

        // Helper Functions
        void set_flag(FLAGS f, bool value);
        bool get_flag(FLAGS f);
        CPU_State get_CPU_state();
        
    private:
        // Registers
        u16 PC;     // Program Counter
        u8 SP;      // Stack Pointer
        u8 A;       // Accumulator
        u8 X;       // Register X
        u8 Y;       // Register Y
        u8 flags;   // All flags

        // Internal States
        u16 addr_abs;       // Absolute address updated by addressing mode functions. This is used to fetch data for the instruction.
        u16 addr_rel;       // Relative address used for branching.
        u8 mem_data;        // Variable to store fetched data for instructions.
        u8 current_cycles;  // Stores how many cycles left for current instruction execution.
        Bus *bus_ptr;           // Pointer to a bus object to read to / write from.

        // Opcode definitions
        struct Instruction {
            const char* name;
            u8 (CPU_6502::*operation)(void);
            u8 (CPU_6502::*addrmode)(void);
            u8 cycles;      
        };
        Instruction opcode_table[256];

        // ---- Instructions ----
        // Load and Stores
        u8 LDA();   // Load Accumulator
        u8 LDX();   // Load Register X
        u8 LDY();   // Load Register Y
        u8 STA();   // Store Accumulator
        u8 STX();   // Store Register X
        u8 STY();   // Store Register Y

        // Register Transfers
        u8 TAX();   // Transfer A to X
        u8 TAY();   // Transfer A to Y
        u8 TXA();   // Transfer X to A
        u8 TYA();   // Transfer Y to A

        // Stack Operations
        u8 TSX();   // Transfer SP to X
        u8 TXS();   // Transfer X to SP
        u8 PHA();   // Push A to stack
        u8 PHP();   // Push flags to stack
        u8 PLA();   // Pull from stack to A
        u8 PLP();   // Pull from stack to flags

        // Logical
        u8 AND();
        u8 EOR();
        u8 ORA();
        u8 BIT();

        // Illegal Opcode
        u8 XXX();

        // Addressing Modes
        u8 IMP();   // Implied
        u8 IMM();   // Immediate
        u8 ZP();    // Zero Page
        u8 ZPX();   // Zero Page, X
        u8 ZPY();   // Zero Page, Y
        u8 ABS();   // Absolute
        u8 ABSX();  // Absolute, X
        u8 ABSY();  // Absolute, Y
        u8 IZX();   // Indexed Indirect
        u8 IZY();   // Indirect Indexed

        // Helper Functions
        void fetch_mem();
        inline void update_zn_flags(u8 reg);
};