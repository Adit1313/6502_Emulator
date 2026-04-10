#include "main.h"
#include "cpu_6502.h"
#include "bus.h"

int main(int argc, char** argv)
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFD, std::vector<u8> {0x2, 0x0}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xA5, 0xA2, 0x4, 0x99, 0x20});
    emu.load_bytes_at_address(0x24, std::vector<u8> {0x12, 0x34});
    emu.reset(emu.RST_CPU);
    
    // SECTION("Verify memory value")
    // {
        emu.execute(10); 
        auto bus = emu.get_bus_obj();
        // REQUIRE(bus.read(0x1234) == 0xA5);
    // }
    auto cpu = emu.get_CPU_obj();
    auto state = cpu.get_CPU_state();
    return 1;
}