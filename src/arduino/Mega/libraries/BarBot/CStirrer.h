#ifndef CSTIRRER_H
#define CSTIRRER_H

#include "CDispenser.h"


#include "Arduino.h"
#include <avr/pgmspace.h>

class CStirrer : public CDispenser
{
  public:
     CStirrer(uint8_t servo_pin);
     ~CStirrer();
     uint8_t          get_dispener_type();
     bool             dispense(uint8_t qty);
     bool             loop();
     dispenser_state  get_status();
     void             stop();
     
  private:
    unsigned long long _dispense_start;
    int _pin;

};

#endif
