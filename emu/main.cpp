#include "main.h"
#include "cpu_6502.h"
#include "bus.h"

int main(int argc, char** argv)
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset.
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0x8F, 0x08, 0xA2, 0x0, 0x08, 0xA2, 0x1, 0x28, 0x28, 0x28});
    emu.reset(emu.RST_CPU);
    emu.execute(16);
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    // REQUIRE(cpu.get_flag(CPU_6502::Z) == 1);
    emu.execute(8);
    cpu = emu.get_CPU_obj();
    state = cpu.get_CPU_state();
    // REQUIRE(state.SP == 0xFD);
    // REQUIRE(cpu.get_flag(CPU_6502::N) == 1);
    return 1;
}