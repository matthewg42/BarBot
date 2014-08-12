#ifndef CCONVEYOR_H
#define CCONVEYOR_H

#include "CDispenser.h"

#include "Arduino.h"
#include <avr/pgmspace.h>


#define CONVEYOR_TIMEOUT        5000 // maximum time to wait (in ms) for a cherry/olive to be dispensed
#define CONVEYOR_REVERSE_TIME   250  // How long to wait after a cherry has been sensed before switching off


class CConveyor : public CDispenser
{
  public:
     CConveyor(uint8_t start_stop, uint8_t end_sensor);
     ~CConveyor();
     uint8_t          get_dispener_type();
     bool             dispense(uint16_t qty);
     bool             loop();
     dispenser_state  get_status();
     void             stop();

  private:
    unsigned long long _dispense_start;
    int _pin_start_stop;
    int _pin_end_sensor;
    unsigned long long _cherry_sensed;

};

#endif
