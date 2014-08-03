#include "CUmbrella.h"

/* Umbrella dispenser
 */


CUmbrella::CUmbrella(uint8_t pin)
{
  _pin = pin;
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, LOW);
  _last_used = millis();
  _state = CUmbrella::IDLE;
  _pulse_sent = false;
}

CUmbrella::~CUmbrella()
{
  digitalWrite(_pin, LOW);
}

uint8_t CUmbrella::get_dispener_type()
{
  return DISPENSER_UMBRELLA;
}

bool CUmbrella::dispense(uint8_t qty)
{
  if (_state != CUmbrella::IDLE)
    return false;

  _state = CUmbrella::BUSY;

  _dispense_start = millis();
  digitalWrite(_pin, HIGH);
  _pulse_sent = false; // Well, it's been started...

  return true;
};

bool CUmbrella::loop()
{
  if (_state != CUmbrella::BUSY)
    return true;

  if ((!_pulse_sent) && millis()-_dispense_start >= UMBRELLA_PULSE_LEN) 
  {
     digitalWrite(_pin, LOW);
     _pulse_sent = true;
  }
  else if (millis()-_dispense_start > (UMBRELLA_PULSE_LEN + UMBRELLA_WAIT))
  {
    // Done!
    _state = CUmbrella::IDLE;
     digitalWrite(_pin, LOW);
  }

  return false;
}

void CUmbrella::stop()
{
   digitalWrite(_pin, LOW);
  _state = CUmbrella::IDLE;
}

CDispenser::dispenser_state CUmbrella::get_status()
{
  return _state;
}
