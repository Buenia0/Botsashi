#include "../m68k.h"
#include <iostream>
#include <functional>
using namespace m68k;
using namespace std;
using namespace std::placeholders;

M68K m68kcpu;

uint8_t memorymap[0x1000000];

uint8_t readb(uint32_t address)
{
    address = (address % 0x1000000);

    return memorymap[address];
}

void writeb(uint32_t address, uint8_t value)
{
    address = (address % 0x1000000);    
 
    memorymap[address] = value;
}

uint16_t readw(uint32_t address)
{
    address = (address % 0x1000000);
    
    return (readb(address) << 8) | readb(address + 1);
}
    
void writew(uint32_t address, uint16_t value)
{
    address = (address % 0x1000000);
        
    writeb((address + 1), (value & 0xFF));
    writeb(address, ((value >> 8) & 0xFF));
}
    
uint32_t readl(uint32_t address)
{
    address = (address % 0x1000000);
        
    return (readw(address) << 16) | readw(address + 2);
}
    
void writel(uint32_t address, uint32_t value)
{
    address = (address % 0x1000000);
        
    writew(address, (value >> 16) & 0xFFFF);
    writew(address + 2, (value & 0xFFFF));
}

void init()
{
    auto rb = bind(readb, _1);
    auto wb = bind(writeb, _1, _2);
    auto rw = bind(readw, _1);
    auto ww = bind(writew, _1, _2);
    auto rl = bind(readl, _1);
    auto wl = bind(writel, _1, _2);

    m68kcpu.setrwbcallback(rb, wb);
    m68kcpu.setrwwcallback(rw, ww);
    m68kcpu.setrwlcallback(rl, wl);
}

int main()
{
    init();
    
    m68kcpu.writeLong(0x000000, 0x0000FFFE);
    m68kcpu.writeLong(0x000004, 0x00000100);
    for (int i = 1; i < 8; i++)
    {
        m68kcpu.writeLong((0x000064 + ((i - 1) * 4)), 0x00000100);
    }
    m68kcpu.writeLong(0x000070, 0x00000130);
    m68kcpu.writeLong(0x000078, 0x00000124);
    m68kcpu.writeLong(0x00007C, 0x00000128);
    m68kcpu.writeWord(0x000100, 0x1039);
    m68kcpu.writeLong(0x000102, 0xA1000001);
    m68kcpu.writeWord(0x000106, 0x70FF);
    m68kcpu.writeWord(0x000108, 0x60F6);

    m68kcpu.reset(0);

    bool quit = false;

    while (!quit)
    {
	   m68kcpu.execute(2060);
	   cout << "Total cycles taken: " << m68kcpu.mcycles << endl;
	   quit = true;	
    }

    m68kcpu.shutdown();

    return 0;
}
