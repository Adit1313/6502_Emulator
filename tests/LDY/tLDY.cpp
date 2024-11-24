#include <BaseTest.h>

class tLDY_Immediate : public BaseTest
{
    public:
    BYTE data;

    tLDY_Immediate(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDY_IMMEDIATE;
        memory[PC_INIT+1] = data;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.Y != data)
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
class tLDY_ZeroPage : public BaseTest
{
    public:
    BYTE data;

    tLDY_ZeroPage(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDY_ZEROPAGE;
        memory[PC_INIT+1] = 0x20;
        memory[0x20] = data;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.Y != data)
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
class tLDY_ZeroPageX : public BaseTest
{
    public:
    BYTE data;

    tLDY_ZeroPageX(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        cpu.X = 0x2;
        memory[PC_INIT] = IntSet::LDY_ZEROPAGEADDX;
        memory[PC_INIT+1] = 0x20;
        memory[0x22] = data;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.Y != data)
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
class tLDY_Absolute: public BaseTest
{
    public:
    BYTE data;

    tLDY_Absolute(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDY_ABSOLUTE;
        memory[PC_INIT+1] = 0xFC;
        memory[PC_INIT+2] = 0xFF;
        memory[0xFFFC] = data;

        // Begin program execution
        Execute(&cpu, &memory, (u32) 1); // Execute n instructions
    }

    virtual void ValidateOutput()
    {
        error = false;
        if (cpu.Y != data)
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
class tLDY_AbsoluteAddX: public BaseTest
{
    public:
    BYTE data;

    tLDY_AbsoluteAddX(BYTE dataReq, u32 cycleNum)
    {
        data = dataReq;
        expectedCycles = cycleNum;
    }

    virtual void ExecuteTest()
    {
        memory[PC_INIT] = IntSet::LDY_ABSOLUTEADDX;
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
        if (cpu.Y != data)
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
        message = "Running testpoint: tLDY_Immediate to verify LDY with immediate addressing mode";
        printTestAnnouncement(message);
        tLDY_Immediate testObj((BYTE) 0x20, (u32)2);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }
    
    {
        message = "Running testpoint: tLDY_Immediate to verify Z flag";
        printTestAnnouncement(message);
        tLDY_Immediate testObj((BYTE) 0x00, (u32)2);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }
    
    {
        message = "Running testpoint: tLDY_Immediate to verify N flag";
        printTestAnnouncement(message);
        tLDY_Immediate testObj((BYTE) 0x81, (u32)2);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tLDY_ZeroPage to verify LDY with zero page addressing mode";
        printTestAnnouncement(message);
        tLDY_ZeroPage testObj((BYTE) 0x20, (u32)3);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tLDY_ZeroPageX to verify LDY with zero page X addressing mode";
        printTestAnnouncement(message);
        tLDY_ZeroPageX testObj((BYTE) 0x20, (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tLDY_Absolute to verify LDY with absolute addressing mode";
        printTestAnnouncement(message);
        tLDY_Absolute testObj((BYTE) 0x29, (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

    {
        message = "Running testpoint: tLDY_AbsoluteAddX to verify LDY with absolute addressing Y mode";
        printTestAnnouncement(message);
        tLDY_AbsoluteAddX testObj((BYTE) 0x29, (u32) 4);
        testObj.ExecuteTest();
        testObj.ValidateOutput();
    }

}