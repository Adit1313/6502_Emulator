#include <BaseTest.h>

class tSTX_ZeroPage : public BaseTest
{
    public:
    BYTE data;
    BYTE address;

    tSTX_ZeroPage(BYTE dataReq, BYTE zpAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = zpAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDX_IMMEDIATE;
        memory[PC_INIT+1] = data;
        memory[PC_INIT+2] = IntSet::STX_ZEROPAGE;
        memory[PC_INIT+3] = address;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 2); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;

        if (memory[address] != data)
        {
            std::cerr << "Incorrect data in memory" << std::endl;
            error = true;
        }

        if (cycles - 2 != expectedCycles)
        {
            std::cerr << "Incorrect number of cycles used" << std::endl;
            error = true;
        }
    }
};
class tSTX_ZeroPageY : public BaseTest
{
    public:
    BYTE data;
    BYTE address;

    tSTX_ZeroPageY(BYTE dataReq, BYTE zpAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = zpAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDX_IMMEDIATE;
        memory[PC_INIT+1] = data;
        memory[PC_INIT+2] = IntSet::LDY_IMMEDIATE;
        memory[PC_INIT+3] = 0x2;
        memory[PC_INIT+4] = IntSet::STX_ZEROPAGEADDY;
        memory[PC_INIT+5] = address;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 3); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (memory[address+0x2] != data)
        {
            std::cerr << "Incorrect data in memory" << std::endl;
            // std::cerr << "Expected data: " << data+48 << " Current data: " << memory[address+0x2] + 48 << std::endl;
            error = true;
        }

        if (cycles - (2 + 2) != expectedCycles)
        {
            std::cerr << "Incorrect number of cycles used" << std::endl;
            error = true;
        }
    }
};
class tSTX_Absolute: public BaseTest
{
    public:
    BYTE data;
    WORD address;

    tSTX_Absolute(BYTE dataReq, WORD absAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = absAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDX_ABSOLUTE;
        memory[PC_INIT+1] = 0xFC;
        memory[PC_INIT+2] = 0xFF;
        memory[0xFFFC] = data;
        memory[PC_INIT+3] = IntSet::STX_ABSOLUTE;
        memory[PC_INIT+4] = (BYTE) (address & 0xFF);
        memory[PC_INIT+5] = (BYTE) (address>>8 & 0xFF);

        // Begin program execution
        Execute(&cpu, &memory, (u32) 2); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (memory[address] != data)
        {
            std::cerr << "Incorrect data in memory" << std::endl;
            error = true;
        }

        if (cycles - 4 != expectedCycles)
        {
            std::cerr << "Incorrect number of cycles used" << std::endl;
            error = true;
        }
    }
};

void printTestAnnouncement(std::string &message)
{
    std::cout << "\e[33m===" << message << "===\e[0m"  << std::endl;
}

int main(void)
{
    std::string message = "";

    {
        message = "Running testpoint: tSTX_ZeroPage to verify STX with zero page addressing mode";
        printTestAnnouncement(message);
        tSTX_ZeroPage testObj((BYTE) 0x20, (BYTE) 0x69, (u32)3);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tSTX_ZeroPageX to verify STX with zero page X addressing mode";
        printTestAnnouncement(message);
        tSTX_ZeroPageY testObj((BYTE) 0x20, (BYTE) 0x69, (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tSTX_Absolute to verify STX with absolute addressing mode";
        printTestAnnouncement(message);
        tSTX_Absolute testObj((BYTE) 0x29, (WORD) 0xFFD0,  (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }
}
