#ifndef CSLICE_H
#define CSLICE_H

#include "CDispenser.h"


#include "Arduino.h"
#include <avr/pgmspace.h>

#define SLICE_TIME   3000 // Time in ms to wait for a slice to be dispensed

class CSlice : public CDispenser
{
  public:
     CSlice(uint8_t servo_pin);
     ~CSlice();
     uint8_t          get_dispener_type();
     bool             dispense(uint8_t qty);
     bool             loop();
     dispenser_state  get_status();
     void             stop();
     
  private:
    unsigned long long _dispense_start;
    int _pin;
    bool _pulse_sent;

};

#endif
