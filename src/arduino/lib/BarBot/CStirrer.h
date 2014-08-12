#ifndef CSTIRRER_H
#define CSTIRRER_H

#include "CDispenser.h"

#define STIRRER_TIME    12000 // Time the stirrer takes in ms
#define STIRRER_PULSE      10 // Pusle duration to trigger stirrer (ms)
#include "Arduino.h"
#include <avr/pgmspace.h>

class CStirrer : public CDispenser
{
  public:
     CStirrer(uint8_t servo_pin);
     ~CStirrer();
     uint8_t          get_dispener_type();
     bool             dispense(uint16_t qty);
     bool             loop();
     dispenser_state  get_status();
     void             stop();
     
  private:
    unsigned long long _dispense_start;
    int _pin;
    bool _pulse_sent;

};

#endif
