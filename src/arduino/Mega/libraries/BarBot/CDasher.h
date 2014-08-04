#ifndef CDASHER_H
#define CDASHER_H

#include "CDispenser.h"

#define DASHER_TIMEOUT 7000 // Max time in ms to enable dasher for

#include "Arduino.h"
#include <avr/pgmspace.h>

class CDasher : public CDispenser
{
  public:
     CDasher(uint8_t cam, uint8_t driver);
     ~CDasher();
     uint8_t          get_dispener_type();
     bool             dispense(uint8_t qty);
     bool             loop();
     dispenser_state  get_status();
     void             stop();
     
  private:
    unsigned long long _dispense_start;
    uint8_t _pin_cam;
    uint8_t _pin_driver;
    uint8_t _qty;
    
    

};

#endif
