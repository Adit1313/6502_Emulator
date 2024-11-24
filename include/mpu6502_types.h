#pragma once

#ifndef MPU6502_TYPES_HEADER
#define MPU6502_TYPES_HEADER

#include <stdio.h>
#include <stdlib.h>

// Defines
#define PC_INIT 0xFF00
#define SP_INIT (BYTE) 0x1FF

// Defining memory sizes
using BYTE = unsigned char;     // One Byte in memory (8-bit)
using WORD = unsigned short;    // One Word in memory (16-bit)

// Defining data types
using u32 = unsigned int;

struct CPU
{
    WORD PC;
    BYTE SP;
    BYTE A, X, Y;
    BYTE C : 1;
    BYTE Z : 1;
    BYTE I : 1;
    BYTE D : 1;
    BYTE B : 1;
    BYTE V : 1;
    BYTE N : 1;
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