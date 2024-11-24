#include <BaseTest.h>

class tLDX_Immediate : public BaseTest
{
    public:
    BYTE data;

    tLDX_Immediate(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDX_IMMEDIATE;
        memory[PC_INIT+1] = data;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.X != data)
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
class tLDX_ZeroPage : public BaseTest
{
    public:
    BYTE data;

    tLDX_ZeroPage(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDX_ZEROPAGE;
        memory[PC_INIT+1] = 0x20;
        memory[0x20] = data;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.X != data)
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
class tLDX_ZeroPageX : public BaseTest
{
    public:
    BYTE data;

    tLDX_ZeroPageX(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        cpu.X = 0x2;
        memory[PC_INIT] = IntSet::LDX_ZEROPAGEADDX;
        memory[PC_INIT+1] = 0x20;
        memory[0x22] = data;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.X != data)
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
class tLDX_Absolute: public BaseTest
{
    public:
    BYTE data;

    tLDX_Absolute(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDX_ABSOLUTE;
        memory[PC_INIT+1] = 0xFC;
        memory[PC_INIT+2] = 0xFF;
        memory[0xFFFC] = data;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.X != data)
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
class tLDX_AbsoluteAddY: public BaseTest
{
    public:
    BYTE data;

    tLDX_AbsoluteAddY(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDX_ABSOLUTEADDY;
        memory[PC_INIT+1] = 0x02;
        memory[PC_INIT+2] = 0x44;
        memory[0x4403] = data;
        cpu.Y = 0x1;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.X != data)
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
class tLDX_AbsoluteAddYWithPageCrossing: public BaseTest
{
    public:
    BYTE data;

    tLDX_AbsoluteAddYWithPageCrossing(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDX_ABSOLUTEADDY;
        memory[PC_INIT+1] = 0x02;
        memory[PC_INIT+2] = 0x44;
        memory[0x4501] = data;
        cpu.Y = 0xFF;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.X != data)
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
        message = "Running testpoint: tLDX_Immediate to verify LDX with immediate addressing mode";
        printTestAnnouncement(message);
        tLDX_Immediate testObj((BYTE) 0x20, (u32)2);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }
    
    {
        message = "Running testpoint: tLDX_Immediate to verify Z flag";
        printTestAnnouncement(message);
        tLDX_Immediate testObj((BYTE) 0x00, (u32)2);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }
    
    {
        message = "Running testpoint: tLDX_Immediate to verify N flag";
        printTestAnnouncement(message);
        tLDX_Immediate testObj((BYTE) 0x81, (u32)2);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tLDX_ZeroPage to verify LDX with zero page addressing mode";
        printTestAnnouncement(message);
        tLDX_ZeroPage testObj((BYTE) 0x20, (u32)3);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tLDX_ZeroPageX to verify LDX with zero page X addressing mode";
        printTestAnnouncement(message);
        tLDX_ZeroPageX testObj((BYTE) 0x20, (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tLDX_Absolute to verify LDX with absolute addressing mode";
        printTestAnnouncement(message);
        tLDX_Absolute testObj((BYTE) 0x29, (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tLDX_AbsoluteAddY to verify LDX with absolute addressing Y mode";
        printTestAnnouncement(message);
        tLDX_AbsoluteAddY testObj((BYTE) 0x29, (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tLDX_AbsoluteAddYWithPageCrossing to verify cycle count for LDX with absolute addressing Y mode with page crossing";
        printTestAnnouncement(message);
        tLDX_AbsoluteAddYWithPageCrossing testObj((BYTE) 0x29, (u32) 5);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

}