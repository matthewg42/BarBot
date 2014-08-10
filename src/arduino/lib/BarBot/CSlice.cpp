#include "CSlice.h"




CSlice::CSlice(uint8_t pin)
{
  _pin = pin;
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);
  _last_used = millis();
  _state = CSlice::IDLE;
  _pulse_sent = false;
}

CSlice::~CSlice()
{
  digitalWrite(_pin, HIGH);
}

uint8_t CSlice::get_dispener_type()
{
  return DISPENSER_SLICE;
}

bool CSlice::dispense(uint8_t qty)
{
  if (_state != CSlice::IDLE)
    return false;

  _state = CSlice::BUSY;

  _dispense_start = millis();
  _pulse_sent = false;  
  digitalWrite(_pin, LOW);

  return true;
}

bool CSlice::loop()
{
  if (_state != CSlice::BUSY)
    return true;
  
  if (!_pulse_sent)
  {
    if (millis()-_dispense_start > SLICE_PULSE)
    {
      _pulse_sent = true;
      digitalWrite(_pin, HIGH);
    }
  } 
  else
  {
    if (millis()-_dispense_start > (SLICE_PULSE + SLICE_TIME))
    {
      // Done!
      _state = CSlice::IDLE;
    }
  }

  return false;
}

void CSlice::stop()
{
  //digitalWrite(_pin, LOW);
  _state = CSlice::IDLE;
}

CDispenser::dispenser_state CSlice::get_status()
{
  return _state;
}
