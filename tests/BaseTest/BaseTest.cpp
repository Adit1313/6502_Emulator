#include <stdlib.h>
#include <iostream>
#include <BaseTest.h>
#include <mpu6502_interface.h>



 void BaseTest::Setup()
    {
        InitOpcodeTable();
        Reset(&cpu, &memory);
        error = false;
    }

void BaseTest::Teardown()
{
    Reset(&cpu, &memory);
    if (error)
    {
        std::cout << "\e[31mTest failed\e[0m" << std::endl;
    }
    else
    {
        std::cout << "\e[32mTest passed\e[0m" << std::endl;
    }
}

BaseTest::BaseTest()
{
    Setup();
}

BaseTest::~BaseTest()
{
    Teardown();
}