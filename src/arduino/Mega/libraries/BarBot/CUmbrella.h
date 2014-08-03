#ifndef CUMBRELLA_H
#define CUMBRELLA_H

#include "CDispenser.h"

#define UMBRELLA_PULSE_LEN 500  // Time to activate the solenoid for
#define UMBRELLA_WAIT      2000 // How long to wait after dropping before being "done"

#include "Arduino.h"
#include <avr/pgmspace.h>

class CUmbrella : public CDispenser
{
  public:
     CUmbrella(uint8_t pin);
     ~CUmbrella();
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
