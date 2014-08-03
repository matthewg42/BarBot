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
  digitalWrite(_pin, LOW);
  _pulse_sent = false; // Well, it's been started...

  return true;
};

bool CSlice::loop()
{
  if (_state != CSlice::BUSY)
    return true;

  if ((!_pulse_sent) && millis()-_dispense_start >= 10) // 10 ms pulse
  {
     digitalWrite(_pin, HIGH);
     _pulse_sent = true;
  }
  else if (millis()-_dispense_start > SLICE_TIME)
  {
    // Done!
    _state = CSlice::IDLE;
  }

  return false;
}

void CSlice::stop()
{
  // the slice dispenser can't be stopped once started
  _state = CSlice::IDLE;
}

CDispenser::dispenser_state CSlice::get_status()
{
  return _state;
}
