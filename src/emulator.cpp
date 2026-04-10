#include "emulator.h"

Emulator::Emulator()
{
    emu_bus.reset();
    emu_cpu.connect_bus(&emu_bus);
    elapsed_cycles = 0;
}

Emulator::~Emulator()
{

}

void Emulator::step()
{
    emu_cpu.clock();
    elapsed_cycles += 1;
}

void Emulator::execute(u32 steps)
{
    while (steps > 0)
    {
        step();
        steps--;
    }
}

void Emulator::reset(RESET_FLAG f)
{
    if (f == RST_EMULATOR)
    {
        emu_bus.reset();
        emu_cpu.reset();
        elapsed_cycles = 0;
    } else if (f == RST_BUS)
    {
        emu_bus.reset();
    } else if (f == RST_CPU)
    {
        emu_cpu.reset();
    } 
}

void Emulator::load_bytes_at_address(u16 address, const std::vector<u8>& data_sequence)
{
    for (u8 idx = 0; idx < data_sequence.size(); idx++)
    {
        emu_bus.write(address+idx, data_sequence[idx]);
    }
}