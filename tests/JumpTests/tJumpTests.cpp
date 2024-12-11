#include <BaseTest.h>

class tJumpAndReturn : public BaseTest
{
    public:
    BYTE data;
    WORD subroutineAddr;
    BYTE resultAddress;

    tJumpAndReturn(BYTE dataReq, WORD jumpAddress, BYTE zpAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        subroutineAddr = jumpAddress;
        resultAddress = zpAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::JSR_ABSOLUTE;
        memory[PC_INIT+1] = subroutineAddr&0xFF;
        memory[PC_INIT+2] = subroutineAddr>>8;
        memory[PC_INIT+3] = IntSet::STA_ZEROPAGE;
        memory[PC_INIT+4] = resultAddress;
        
        memory[subroutineAddr] = IntSet::LDA_IMMEDIATE;
        memory[subroutineAddr+1] = data;
        memory[subroutineAddr+2] = IntSet::RTS;
        
        // Begin program execution
        Execute(&cpu, &memory, (u32) 4); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;

        if (memory[resultAddress] != data)
        {
            std::cerr << "Incorrect data in memory" << std::endl;
            error = true;
        }

        if (cycles - (3+2) != expectedCycles)
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
        tJumpAndReturn testObj((BYTE) 0x20, (WORD) 0xFD00, (BYTE) 0x30, (u32)12);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }
}

