#pragma once

#include "common.h"
#include "cpu_6502.h"
#include "bus.h"

class CPU_6502;
class Bus;

class Emulator
{
    private:
        CPU_6502 emu_cpu;
        Bus emu_bus;

    public:
        Emulator();
        ~Emulator();

        // Enums
        enum RESET_FLAG {
            RST_EMULATOR = 0,
            RST_CPU = 1,
            RST_BUS = 2
        };

        enum EXECUTION_MODE {
            CYCLES = 0,
            INSTRUCTIONS = 1
        };

        // Functions
        void load_bytes_at_address(u16 address, const std::vector<u8>& data_sequence);
        void step(); // Executes a single step of the emulator
        void execute(u32 count, EXECUTION_MODE mode = CYCLES); // Executes n cycles (default) or n full instructions
        void reset(RESET_FLAG f);

        // Helper Functions
        const CPU_6502& get_CPU_obj() const {return emu_cpu;}
        const Bus& get_bus_obj() const {return emu_bus;}

        // Variables
        u32 elapsed_cycles;
};