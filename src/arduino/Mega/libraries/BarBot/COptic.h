#ifndef COPTIC_H
#define COPTIC_H

#define OPTIC_IDLE_POSITION       90
#define OPTIC_DISPENSE_POSITION    0
#define OPTIC_DISPENSE_TIME     3000 // ms to activate servo for when dispensing
#define OPTIC_RECHARGE_TIME     2500 // minimum time to wait between dispenses 

#include "COptic.h"
#include "CDispenser.h"

#include "../../../../libraries/Servo/Servo.h"

#include "Arduino.h"
#include <avr/pgmspace.h>

class COptic : public CDispenser
{
  public:
     COptic(uint8_t servo_pin);
     ~COptic();
     uint8_t          get_dispener_type();
     bool             dispense(uint8_t qty);
     bool             loop();
     dispenser_state  get_status();
     void             stop();
     
  private:
    Servo _servo;
    unsigned long long _dispense_start;
    bool _dispense_started;
  
};

#endif
