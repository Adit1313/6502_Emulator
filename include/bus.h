#pragma once

#include "common.h"

class Bus
{
    public:
        Bus();
        ~Bus();

        void write(u16 address, u8 value);
        u8 read(u16 address);
        void reset();

    private:
        std::array<uint8_t, 64 * 1024> ram;
};