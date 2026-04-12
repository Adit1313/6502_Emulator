#include "emulator.h"
#include <catch2/catch_test_macros.hpp>
#include <cstdio>

#pragma region STA Tests
TEST_CASE("STA with ZP addressing", "[STA][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xA5, 0x85, 0x20});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(5); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x20) == 0xA5);
    }
}

TEST_CASE("STA with ZPX addressing", "[STA][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xA5, 0xA2, 0x1, 0x95, 0x1F});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(6); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x20) == 0xA5);
    }
}

TEST_CASE("STA with ABS addressing", "[STA][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xA5, 0x8D, 0x34, 0x12});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(6); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x1234) == 0xA5);
    }
}

TEST_CASE("STA with ABSX addressing", "[STA][ABSX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xA5, 0xA2, 0x1, 0x9D, 0x33, 0x12});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(9); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x1234) == 0xA5);
    }
}

TEST_CASE("STA with ABSY addressing", "[STA][ABSY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xA5, 0xA0, 0x1, 0x99, 0x33, 0x12});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(9); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x1234) == 0xA5);
    }
}

TEST_CASE("STA with IZX addressing", "[STA][IZX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xA5, 0xA2, 0x4, 0x81, 0x20});
    emu.load_bytes_at_address(0x24, std::vector<u8> {0x34, 0x12});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(10); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x1234) == 0xA5);
    }
}

TEST_CASE("STA with IZY addressing", "[STA][IZX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA9, 0xA5, 0xA0, 0x4, 0x91, 0x20});
    emu.load_bytes_at_address(0x20, std::vector<u8> {0x30, 0x12});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(10); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x1234) == 0xA5);
    }
}

#pragma endregion

#pragma region STX Tests

TEST_CASE("STX with ZP addressing", "[STX][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0xA5, 0x86, 0x20});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(5); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x20) == 0xA5);
    }
}

TEST_CASE("STX with ZPY addressing", "[STX][ZPY]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0xA5, 0xA0, 0x1, 0x96, 0x1F});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(6); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x20) == 0xA5);
    }
}

TEST_CASE("STX with ABS addressing", "[STX][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA2, 0xA5, 0x8E, 0x34, 0x12});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(6); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x1234) == 0xA5);
    }
}

#pragma endregion

#pragma region STY Tests

TEST_CASE("STY with ZP addressing", "[STY][ZP]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0xA5, 0x84, 0x20});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(5); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x20) == 0xA5);
    }
}

TEST_CASE("STY with ZPX addressing", "[STY][ZPX]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0xA5, 0xA2, 0x1, 0x94, 0x1F});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(6); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x20) == 0xA5);
    }
}

TEST_CASE("STY with ABS addressing", "[STY][ABS]")
{
    Emulator emu;
    emu.load_bytes_at_address(0xFFFC, std::vector<u8> {0x0, 0x2}); // Tells the CPU where to go after reset. PC breaks without this
    emu.load_bytes_at_address(0x200, std::vector<u8> {0xA0, 0xA5, 0x8C, 0x34, 0x12});
    emu.reset(emu.RST_CPU);
    
    SECTION("Verify memory value")
    {
        emu.execute(6); 
        auto bus = emu.get_bus_obj();
        REQUIRE(bus.read(0x1234) == 0xA5);
    }
}

#pragma endregion