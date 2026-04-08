#include "bus.h"

Bus::Bus()
{
    ram.fill(0);
}

Bus::~Bus()
{

}

u8 Bus::read(u16 address)
{
    return ram[address];
}

void Bus::write(u16 address, u8 data)
{
    ram[address] = data;
}