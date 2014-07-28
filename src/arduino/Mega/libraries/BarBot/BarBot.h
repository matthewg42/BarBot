/*
  
*/
#ifndef BarBot_H
#define BarBot_H

#include "Arduino.h"
#include <avr/pgmspace.h>


class BarBot
{
  public:
    enum instruction_type
    {
      MOVE,
      DISPENSE,
      WAIT
    };
         
    BarBot();
    bool instruction_add(instruction_type instruction, uint16_t param1);
    bool instructions_clear();
    bool go();
    bool loop();

    
  private:
    short int c;
};

#endif