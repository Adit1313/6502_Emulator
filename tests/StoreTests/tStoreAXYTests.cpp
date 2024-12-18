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
class tSTA_IndirectX: public BaseTest
{
    public:
    BYTE data;
    WORD address;

    tSTA_IndirectX(BYTE dataReq, WORD dAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = dAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDA_IMMEDIATE;
        memory[PC_INIT+1] = data;
        memory[PC_INIT+2] = IntSet::LDX_IMMEDIATE;
        memory[PC_INIT+3] = 0x10;
        memory[PC_INIT+4] = IntSet::STA_INDIRECTX;
        memory[PC_INIT+5] = 0x20;
        memory[0x0030] = (BYTE) (address & 0xFF);
        memory[0x0031] = (address>>8 & 0xFF);

        // Begin program execution
        Execute(&cpu, &memory, (u32) 3); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (memory[address] != data)
        {
            std::cerr << "Incorrect data in memory" << std::endl;
            error = true;
        }

        if (cycles - (2+2) != expectedCycles)
        {
            std::cerr << "Incorrect number of cycles used" << std::endl;
            error = true;
        }
    }
};
class tSTA_IndirectY: public BaseTest
{
    public:
    BYTE data;
    WORD address;

    tSTA_IndirectY(BYTE dataReq, WORD dAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = dAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDA_IMMEDIATE;
        memory[PC_INIT+1] = data;
        memory[PC_INIT+2] = IntSet::LDY_IMMEDIATE;
        memory[PC_INIT+3] = 0x13;
        memory[PC_INIT+4] = IntSet::STA_INDIRECTY;
        memory[PC_INIT+5] = 0x20;
        memory[0x0020] = (BYTE) (address & 0xFF);
        memory[0x0021] = (address>>8 & 0xFF);

        // Begin program execution
        Execute(&cpu, &memory, (u32) 3); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (memory[address+0x13] != data)
        {
            std::cerr << "Incorrect data in memory" << std::endl;
            error = true;
        }

        if (cycles - (2+2) != expectedCycles)
        {
            std::cerr << "Incorrect number of cycles used" << std::endl;
            error = true;
        }
    }
};
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
class tSTY_ZeroPage : public BaseTest
{
    public:
    BYTE data;
    BYTE address;

    tSTY_ZeroPage(BYTE dataReq, BYTE zpAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = zpAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDY_IMMEDIATE;
        memory[PC_INIT+1] = data;
        memory[PC_INIT+2] = IntSet::STY_ZEROPAGE;
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
class tSTY_ZeroPageX : public BaseTest
{
    public:
    BYTE data;
    BYTE address;

    tSTY_ZeroPageX(BYTE dataReq, BYTE zpAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = zpAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDY_IMMEDIATE;
        memory[PC_INIT+1] = data;
        memory[PC_INIT+2] = IntSet::LDX_IMMEDIATE;
        memory[PC_INIT+3] = 0x2;
        memory[PC_INIT+4] = IntSet::STY_ZEROPAGEADDX;
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
class tSTY_Absolute: public BaseTest
{
    public:
    BYTE data;
    WORD address;

    tSTY_Absolute(BYTE dataReq, WORD absAddress, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
        address = absAddress;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDY_ABSOLUTE;
        memory[PC_INIT+1] = 0xFC;
        memory[PC_INIT+2] = 0xFF;
        memory[0xFFFC] = data;
        memory[PC_INIT+3] = IntSet::STY_ABSOLUTE;
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

    {
        message = "Running testpoint: tSTA_IndirectX to verify STA with indirect X addressing mode";
        printTestAnnouncement(message);
        tSTA_IndirectX testObj((BYTE) 0x29, (WORD) 0xFFD0, (u32) 6);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tSTA_IndirectY to verify STA with indirect Y addressing mode";
        printTestAnnouncement(message);
        tSTA_IndirectY testObj((BYTE) 0x29, (WORD) 0xFFD0, (u32) 6);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

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

    {
        message = "Running testpoint: tSTY_ZeroPage to verify STY with zero page addressing mode";
        printTestAnnouncement(message);
        tSTY_ZeroPage testObj((BYTE) 0x20, (BYTE) 0x69, (u32)3);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tSTY_ZeroPageX to verify STY with zero page X addressing mode";
        printTestAnnouncement(message);
        tSTY_ZeroPageX testObj((BYTE) 0x20, (BYTE) 0x69, (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tSTY_Absolute to verify STY with absolute addressing mode";
        printTestAnnouncement(message);
        tSTY_Absolute testObj((BYTE) 0x29, (WORD) 0xFFD0,  (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }
}

