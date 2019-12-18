#ifndef BOTSASHI_PRECISE
#define BOTSASHI_PRECISE

#include <array>
#include <functional>
#include <iostream>
#include <unordered_map>
#include "botsashi.h"
using namespace std;
using namespace botsashi;

namespace botsashi
{
  inline void m68kmovb(Botsashi *m68k)
  {
    // cout << "MOV.B" << endl;
    int addrmode = ((m68k->currentinstr >> 3) & 0x7);
    int addrreg = (m68k->currentinstr & 0x7);
    uint8_t temp = (uint8_t)((m68k->srcaddressingmode(addrmode, addrreg, BYTE_WIDTH)) & 0xFF);
    // cout << hex << (int)(temp) << endl;
    int destmode = ((m68k->currentinstr >> 6) & 0x7);
    int destreg = ((m68k->currentinstr >> 9) & 0x7);
    m68k->dstaddressingmode(destmode, destreg, BYTE_WIDTH, temp);
    m68k->m68kreg.setflags(m68k->m68kreg.getx(), TestBit(temp, 7), (temp == 0), false, false);
  }

  inline void m68kmovw(Botsashi *m68k)
  {
    // cout << "MOV.W" << endl;
    int addrmode = ((m68k->currentinstr >> 3) & 0x7);
    int addrreg = (m68k->currentinstr & 0x7);
    uint16_t temp = (uint16_t)((m68k->srcaddressingmode(addrmode, addrreg, WORD_WIDTH)) & 0xFFFF);
    // cout << hex << (int)(temp) << endl;
    int destmode = ((m68k->currentinstr >> 6) & 0x7);
    int destreg = ((m68k->currentinstr >> 9) & 0x7);
    m68k->dstaddressingmode(destmode, destreg, WORD_WIDTH, temp);
    m68k->m68kreg.setflags(m68k->m68kreg.getx(), TestBit(temp, 15), (temp == 0), false, false);
  }

  inline void m68kmovl(Botsashi *m68k)
  {
    // cout << "MOV.L" << endl;
    int addrmode = ((m68k->currentinstr >> 3) & 0x7);
    int addrreg = (m68k->currentinstr & 0x7);
    uint32_t temp = (uint32_t)(m68k->srcaddressingmode(addrmode, addrreg, LONG_WIDTH));
    // cout << hex << (int)(temp) << endl;
    int destmode = ((m68k->currentinstr >> 6) & 0x7);
    int destreg = ((m68k->currentinstr >> 9) & 0x7);
    m68k->dstaddressingmode(destmode, destreg, LONG_WIDTH, temp);
    m68k->m68kreg.setflags(m68k->m68kreg.getx(), TestBit(temp, 31), (temp == 0), false, false);
  }

  inline void m68klea(Botsashi *m68k)
  {
    // cout << "LEA" << endl;
    int addrmode = ((m68k->currentinstr >> 3) & 0x7);
    int addrreg = (m68k->currentinstr & 0x7);
    uint32_t temp = (m68k->srcaddressingmode(addrmode, addrreg, LONG_WIDTH));
    int destaddrreg = ((m68k->currentinstr >> 9) & 0x7);
    m68k->m68kreg.setaddrreg(destaddrreg, temp);
  }

  inline void m68ktrap(Botsashi *m68k)
  {
    // cout << "TRAP" << endl;
    m68k->trapexcep((m68k->currentinstr & 0xF), m68k);
  }

  inline void m68kstop(Botsashi *m68k)
  {
    // cout << "STOP" << endl;
    m68k->m68kreg.sr = m68k->readWord(m68k->m68kreg.pc);
    m68k->m68kreg.pc += 2;
    m68k->stopped = true;
  }

  inline void m68kbranch(Botsashi *m68k)
  {
    int temp = ((m68k->currentinstr >> 8) & 0xF);

    if (temp == 0)
    {
      // cout << "BRA" << endl;
      uint8_t tempdisp = (m68k->currentinstr & 0xFF);

      if (tempdisp == 0)
      {
        int16_t disp16 = (int16_t)(m68k->readWord(m68k->m68kreg.pc));
        m68k->m68kreg.pc += disp16;
      }
      else if (tempdisp == 0xFF)
      {
        int32_t disp32 = (int32_t)(m68k->readLong(m68k->m68kreg.pc));
        m68k->m68kreg.pc += disp32;
      }
      else
      {
        int8_t disp8 = (int8_t)(tempdisp);
        m68k->m68kreg.pc += disp8;
      }
    }
    else if (temp == 1)
    {
      // cout << "BSR" << endl;
      exit(1);
    }
    else
    {
      // cout << dec << (int)(m68k->m68kreg.pc) << endl;

      if (m68k->getcond(temp))
      {
        uint8_t tempdisp = (m68k->currentinstr & 0xFF);

        if (tempdisp == 0)
        {
          int16_t disp16 = (int16_t)(m68k->readWord(m68k->m68kreg.pc));
          m68k->m68kreg.pc += disp16;
        }
        else if (tempdisp == 0xFF)
        {
          int32_t disp32 = (int32_t)(m68k->readLong(m68k->m68kreg.pc));
          m68k->m68kreg.pc += disp32;
        }
        else
        {
          int8_t disp8 = (int8_t)(tempdisp);
          m68k->m68kreg.pc += disp8;
        }
      }

      // cout << dec << (int)(m68k->m68kreg.pc) << endl;
    }
  }
}


#endif // BOTSASHI_PRECISE