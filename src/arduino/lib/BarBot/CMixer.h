#ifndef CMIXER_H
#define CMIXER_H

#define MIXER_IDLE_POSITION       140
#define MIXER_DISPENSE_POSITION    65
#define ML_PER_MS                  36  // how many ms to dispense 1ml


#include "CMixer.h"
#include "CDispenser.h"

#include "../../../../libraries/Servo/Servo.h"

#include "Arduino.h"
#include <avr/pgmspace.h>

class CMixer : public CDispenser
{
  public:
     CMixer(uint8_t servo_pin);
     ~CMixer();
     uint8_t          get_dispener_type();
     bool             dispense(uint8_t qty);
     bool             loop();
     dispenser_state  get_status();
     void             stop();
     
  private:
    Servo _servo;
    unsigned long long _dispense_start;
    uint16_t _dispense_time; // how long in ms to dispense for
  
};

#endif
