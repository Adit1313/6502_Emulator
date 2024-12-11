#pragma once

#ifndef MPU6502_TYPES_HEADER
#define MPU6502_TYPES_HEADER

#include <stdio.h>
#include <stdlib.h>

// Defines
#define PC_INIT 0xFF00
#define SP_INIT (BYTE) 0xFF

// Defining memory sizes
using BYTE = unsigned char;     // One Byte in memory (8-bit)
using WORD = unsigned short;    // One Word in memory (16-bit)

// Defining data types
using u32 = unsigned int;

struct CPU
{
    WORD PC;        // Program Counter
    BYTE SP;        // Stack Pointer
    BYTE A, X, Y;   // Accumulator, Index Register X & Y
    BYTE C : 1;     // Carry        1 = True
    BYTE Z : 1;     // Zero         1 = Result 0
    BYTE I : 1;     // IRQ Disable  1 = Disabled
    BYTE D : 1;     // Decimal Mode 1 = True
    BYTE B : 1;     // BRK Command  1 = Break
    BYTE V : 1;     // Overflow     1 = True
    BYTE N : 1;     // Negative     1 = Negative
};

// Defining our memory
struct Mem {
    static constexpr u32 MAX_MEM = 1024 * 64;
    BYTE Data[MAX_MEM];

    BYTE operator[](u32 address) const
    {
        return Data[address];
    }

    BYTE& operator[](u32 address)
    {
        return Data[address];
    }
};

// Defining variables
extern u32 cycles;

#endif