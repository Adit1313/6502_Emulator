#include <mpu6502_interface.h>

//Functions with behaviour for CPU
// Reset - Reset all CPU registers. Initialize the memory.
void Reset(CPU *cpu, Mem *memory)
{
    cpu->A = cpu->X = cpu->Y = 0;
    cpu->PC = PC_INIT;
    cpu->SP = SP_INIT; 
    cpu->C = cpu->Z = cpu->I = cpu->D = cpu->B = cpu->V = cpu->N = 0;

    initMemory(memory);
    InitOpcodeTable();
    cycles = 0;
}

// Execute - Runs the CPU for a given number of cycles
void Execute(CPU *cpu, Mem *memory, u32 instructionLimit)
{
    while (instructionLimit > 0)
    {
        instructionLimit -= 1;
        OpcodeEntry opcode = opcodeTable[FetchByte(cpu, memory)];           // Fetch the opcode from memory
                                                                            //  This contains information on the instruction
                                                                            //  and the addressing mode used.
        OperandType opType;
        OperandValue opVal = opcode.addressModeFcn(cpu, memory, &opType);   // Fetch the operand using the addressing mode

        opcode.instrFcn(cpu, memory, opVal, opType);                        // Execute the required instruction
    }
}
