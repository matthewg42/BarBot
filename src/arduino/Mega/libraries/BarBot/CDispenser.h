/*
  
*/
#ifndef CDISPENSER_H
#define CDISPENSER_H

#include "Arduino.h"
#include <avr/pgmspace.h>
void debug(char *msg);

class CDispenser
{
  public:


    enum dispenser_type
    {
      DISPENSER_OPTIC    = 1,
      DISPENSER_MIXER    = 2,
      DISPENSER_DASHER   = 3,
      DISPENSER_SYRINGE  = 4,
      DISPENSER_CONVEYOR = 5,
      DISPENSER_STIRRER  = 6,
      DISPENSER_SLICE    = 7,
      DISPENSER_UMBRELLA = 8
    };    
    
    enum dispenser_state
    {
      IDLE,
      BUSY,
      FAULT
    };

    CDispenser();
    virtual uint8_t         get_dispener_type()   = 0;
    virtual bool            dispense(uint8_t qty) = 0;
    virtual bool            loop()                = 0;
    virtual dispenser_state get_status()          = 0; 
    virtual void            stop()                = 0;

    
  protected:
    unsigned long long _last_used;
    dispenser_state _state;
    
    
};

#endif
