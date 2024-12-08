#include <BaseTest.h>

class tJumpToSubRoutine : public BaseTest
{
    public:
    BYTE data;
    BYTE address;

    tJumpToSubRoutine(BYTE dataReq, BYTE zpAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = zpAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDA_IMMEDIATE;
        memory[PC_INIT+1] = data;
        memory[PC_INIT+2] = IntSet::STA_ZEROPAGE;
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

void printTestAnnouncement(std::string &message)
{
    std::cout << "\e[33m===" << message << "===\e[0m"  << std::endl;
}

int main(void)
{
    std::string message = "";

    {
        message = "Running testpoint: tSTA_ZeroPage to verify STA with zero page addressing mode";
        printTestAnnouncement(message);
        tJumpToSubRoutine testObj((BYTE) 0x20, (BYTE) 0x69, (u32)3);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }
}

