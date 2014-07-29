/*
  
*/
#ifndef BARBOT_H
#define BARBOT_H

#include "CDispenser.h"
#include "COptic.h"

#include "Arduino.h"
#include <avr/pgmspace.h>

#define MAX_INSTRUCTIONS 100

#define DISPENSER_COUNT  21

void debug(char *msg);


class BarBot
{
  public:
    enum instruction_type
    {
      NOP,
      MOVE,
      DISPENSE,
      WAIT
    };
    
    enum barbot_state
    {
      IDLE,
      RUNNING,
      FAULT
    };
    
    BarBot();
    ~BarBot();
    bool instruction_add(instruction_type instruction, uint16_t param1, uint16_t param2);
    bool instructions_clear();
    bool go();
    bool loop();
    

    
  private:
    struct instruction
    {
      instruction_type  type;
      uint16_t          param1;
      uint16_t          param2;
    };
         
    bool exec_instruction(uint16_t instruction);
    
    barbot_state _state;
    instruction _instructions[MAX_INSTRUCTIONS];
    uint16_t _instruction_count;
    uint16_t _current_instruction;
    unsigned long long _wait_inst_start;
    
    CDispenser *_dispeners[DISPENSER_COUNT];
};

#endif
