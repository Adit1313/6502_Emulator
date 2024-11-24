#include <stdlib.h>
#include <iostream>
#include <mpu6502_interface.h>

class BaseTest
{
    public:
    CPU cpu;
    Mem memory;
    bool error;
    u32 expectedCycles;

    virtual void Setup();

    virtual void ExecuteTest() = 0;
    virtual void ValidateOutput() = 0;

    virtual void Teardown();

    BaseTest();

    ~BaseTest();
};