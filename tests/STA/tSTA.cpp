#include <BaseTest.h>

class tSTA_ZeroPage : public BaseTest
{
    public:
    BYTE data;
    BYTE address;

    tSTA_ZeroPage(BYTE dataReq, BYTE zpAddress, u32 cycleNum)
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
class tSTA_ZeroPageX : public BaseTest
{
    public:
    BYTE data;
    BYTE address;

    tSTA_ZeroPageX(BYTE dataReq, BYTE zpAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = zpAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDA_IMMEDIATE;
        memory[PC_INIT+1] = data;
        memory[PC_INIT+2] = IntSet::LDX_IMMEDIATE;
        memory[PC_INIT+3] = 0x2;
        memory[PC_INIT+4] = IntSet::STA_ZEROPAGEADDX;
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
class tSTA_Absolute: public BaseTest
{
    public:
    BYTE data;
    WORD address;

    tSTA_Absolute(BYTE dataReq, WORD absAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = absAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDA_ABSOLUTE;
        memory[PC_INIT+1] = 0xFC;
        memory[PC_INIT+2] = 0xFF;
        memory[0xFFFC] = data;
        memory[PC_INIT+3] = IntSet::STA_ABSOLUTE;
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
class tSTA_AbsoluteAddX: public BaseTest
{
    public:
    BYTE data;
    WORD address;
    
    tSTA_AbsoluteAddX(BYTE dataReq, WORD absAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = absAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDA_ABSOLUTE;
        memory[PC_INIT+1] = 0xFC;
        memory[PC_INIT+2] = 0xFF;
        memory[0xFFFC] = data;
        memory[PC_INIT+3] = IntSet::LDX_IMMEDIATE;
        memory[PC_INIT+4] = 0x2;
        memory[PC_INIT+5] = IntSet::STA_ABSOLUTEADDX;
        memory[PC_INIT+6] = (BYTE) (address & 0xFF);
        memory[PC_INIT+7] = (BYTE) (address>>8 & 0xFF);

        // Begin program execution
        Execute(&cpu, &memory, (u32) 3); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (memory[address+0x2] != data)
        {
            std::cerr << "Incorrect data in memory" << std::endl;
            error = true;
        }

        if (cycles - (4+2) != expectedCycles)
        {
            std::cerr << "Incorrect number of cycles used" << std::endl;
            error = true;
        }
    }
};
class tSTA_AbsoluteAddY: public BaseTest
{
    public:
    BYTE data;
    WORD address;
    
    tSTA_AbsoluteAddY(BYTE dataReq, WORD absAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = absAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDA_ABSOLUTE;
        memory[PC_INIT+1] = 0xFC;
        memory[PC_INIT+2] = 0xFF;
        memory[0xFFFC] = data;
        memory[PC_INIT+3] = IntSet::LDY_IMMEDIATE;
        memory[PC_INIT+4] = 0x2;
        memory[PC_INIT+5] = IntSet::STA_ABSOLUTEADDY;
        memory[PC_INIT+6] = (BYTE) (address & 0xFF);
        memory[PC_INIT+7] = (BYTE) (address>>8 & 0xFF);

        // Begin program execution
        Execute(&cpu, &memory, (u32) 3); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (memory[address+0x2] != data)
        {
            std::cerr << "Incorrect data in memory" << std::endl;
            error = true;
        }

        if (cycles - (4+2) != expectedCycles)
        {
            std::cerr << "Incorrect number of cycles used" << std::endl;
            error = true;
        }
    }
};
class tLDA_IndirectX: public BaseTest
{
    public:
    BYTE data;

    tLDA_IndirectX(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDA_INDIRECTX;
        memory[PC_INIT+1] = 0x20;
        cpu.X = 0x10;
        memory[0x0030] = 0xFF;
        memory[0x0031] = 0x44;
        memory[0x44FF] = data;
        

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.A != data)
        {
            std::cerr << "Incorrect data in Accumulator" << std::endl;
            error = true;
        }

        if (data==0)
        {
            if (cpu.Z != 1)
            {
                std::cerr << "Incorrect value of Z flag" << std::endl;
                error = true;
            }
        }

        if (data &0b10000000 > 0)
        {
            if (cpu.N != 1)
            {
                std::cerr << "Incorrect value of N flag" << std::endl;
                error = true;
            }
        }

        if (cycles != expectedCycles)
        {
            std::cerr << "Incorrect number of cycles used" << std::endl;
            error = true;
        }
    }
};
class tLDA_IndirectY: public BaseTest
{
    public:
    BYTE data;

    tLDA_IndirectY(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDA_INDIRECTY;
        memory[PC_INIT+1] = 0x20;
        cpu.Y = 0x01;
        memory[0x0020] = 0x02;
        memory[0x0021] = 0x44;
        memory[0x4403] = data;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.A != data)
        {
            std::cerr << "Incorrect data in Accumulator" << std::endl;
            error = true;
        }

        if (data==0)
        {
            if (cpu.Z != 1)
            {
                std::cerr << "Incorrect value of Z flag" << std::endl;
                error = true;
            }
        }

        if (data &0b10000000 > 0)
        {
            if (cpu.N != 1)
            {
                std::cerr << "Incorrect value of N flag" << std::endl;
                error = true;
            }
        }

        if (cycles != expectedCycles)
        {
            std::cerr << "Incorrect number of cycles used" << std::endl;
            error = true;
        }
    }
};
class tLDA_IndirectYWithPageCrossing: public BaseTest
{
    public:
    BYTE data;

    tLDA_IndirectYWithPageCrossing(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDA_INDIRECTY;
        memory[PC_INIT+1] = 0x20;
        cpu.Y = 0xFF;
        memory[0x0020] = 0x02;
        memory[0x0021] = 0x44;
        memory[0x4501] = data;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.A != data)
        {
            std::cerr << "Incorrect data in Accumulator" << std::endl;
            error = true;
        }

        if (data==0)
        {
            if (cpu.Z != 1)
            {
                std::cerr << "Incorrect value of Z flag" << std::endl;
                error = true;
            }
        }

        if (data &0b10000000 > 0)
        {
            if (cpu.N != 1)
            {
                std::cerr << "Incorrect value of N flag" << std::endl;
                error = true;
            }
        }

        if (cycles != expectedCycles)
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
        tSTA_ZeroPage testObj((BYTE) 0x20, (BYTE) 0x69, (u32)3);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tSTA_ZeroPageX to verify STA with zero page X addressing mode";
        printTestAnnouncement(message);
        tSTA_ZeroPageX testObj((BYTE) 0x20, (BYTE) 0x69, (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tSTA_Absolute to verify STA with absolute addressing mode";
        printTestAnnouncement(message);
        tSTA_Absolute testObj((BYTE) 0x29, (WORD) 0xFFD0,  (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tSTA_AbsoluteAddX to verify STA with absolute addressing X mode";
        printTestAnnouncement(message);
        tSTA_AbsoluteAddX testObj((BYTE) 0x29, (WORD) 0xFFD0, (u32) 5);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tSTA_AbsoluteAddY to verify STA with absolute addressing Y mode";
        printTestAnnouncement(message);
        tSTA_AbsoluteAddY testObj((BYTE) 0x29, (WORD) 0xFFD0, (u32) 5);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    // {
    //     message = "Running testpoint: tLDA_AbsoluteAddYWithPageCrossing to verify cycle count for LDA with absolute addressing Y mode with page crossing";
    //     printTestAnnouncement(message);
    //     tLDA_AbsoluteAddYWithPageCrossing testObj((BYTE) 0x29, (u32) 5);
    //     testObj.ExecuteTest();
    //     testObj.ValidateOutput();
    // }

    // {
    //     message = "Running testpoint: tLDA_IndirectX to verify LDA with indirect X addressing mode";
    //     printTestAnnouncement(message);
    //     tLDA_IndirectX testObj((BYTE) 0x29, (u32) 6);
    //     testObj.ExecuteTest();
    //     testObj.ValidateOutput();
    // }

    // {
    //     message = "Running testpoint: tLDA_IndirectY to verify LDA with indirect Y addressing mode";
    //     printTestAnnouncement(message);
    //     tLDA_IndirectY testObj((BYTE) 0x29, (u32) 5);
    //     testObj.ExecuteTest();
    //     testObj.ValidateOutput();
    // }

    // {
    //     message = "Running testpoint: tLDA_IndirectYWithPageCrossing to verify LDA with indirect Y addressing mode";
    //     printTestAnnouncement(message);
    //     tLDA_IndirectYWithPageCrossing testObj((BYTE) 0x29, (u32) 6);
    //     testObj.ExecuteTest();
    //     testObj.ValidateOutput();
    // }
}

