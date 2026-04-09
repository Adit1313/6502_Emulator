#pragma once

#include <stdint.h>
#include <array>
#include <vector>

#define u64 uint64_t
#define u32 uint32_t
#define u16 uint16_t
#define u8 uint8_t

#define SET_BIT(reg, bit) reg |= (1<<bit)
#define CLEAR_BIT(reg, bit) reg &= ~(1<<bit)
#define GET_BIT(reg, bit) (reg & (1<<bit)) >> bit