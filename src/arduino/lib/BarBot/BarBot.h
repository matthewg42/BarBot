/*
  
*/
#ifndef BARBOT_H
#define BARBOT_H

#include "CDispenser.h"
#include "COptic.h"
#include "CStirrer.h"
#include "CConveyor.h"
#include "CUmbrella.h"
#include "CSlice.h"
#include "CMixer.h"
#include "CDasher.h"

#include "Arduino.h"
#include "AccelStepper.h"
#include <avr/pgmspace.h>

#define MAX_INSTRUCTIONS    100  // Maximum number of instructions that can be stored

#define MAX_MOVE_TIME      19000 // Maximum amount of time moving the platform should take (in ms).
#define STEPS_PER_CM         48  // Number of steps per CM (platform movement)
#define MAX_RAIL_POSITION  7010  // Maximum number of steps

// Harware setup
#define DISPENSER_COUNT      21   // Number of attached dispensers. If altered, also need to change BarBot::BarBot()
#define ZERO_SWITCH          52   // Zero/limit switch
#define ESTOP_PIN            53   // Emergency stop 
#define SPEED_ZERO          1200  // Speed when zeroing
#define SPEED_NORMAL        1500  // Normal speed
#define MAX_ACCEL           3000

void debug(char *msg);


class BarBot
{
  public:
    enum instruction_type
    {
      NOP,
      MOVE,       // Move to position <param1>
      DISPENSE,   // Dispense using dispenser <param1> with <param2>
      WAIT,       // Wait for <param1> ms
      ZERO        // Move platform until it hits the limit switch, then call that 0
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
    bool reset();
    bool loop();
    barbot_state get_state();
    
    

      
  private:
    struct instruction
    {
      instruction_type  type;
      uint16_t          param1;
      uint16_t          param2;
    };
         
    bool exec_instruction(uint16_t instruction);
    void move_to(long pos);
    void set_state(barbot_state state);
    
    barbot_state _state;
    instruction _instructions[MAX_INSTRUCTIONS];
    uint16_t _instruction_count;
    uint16_t _current_instruction;
    unsigned long long _wait_inst_start;
    unsigned long long _move_start;
    AccelStepper *_stepper;
    long _stepper_target;
    CDispenser *_dispeners[DISPENSER_COUNT];
};

#endif
