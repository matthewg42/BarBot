#include "CStirrer.h"

/* Stirrer dispenser
 * Sends a ~10ms pulse to start the stirrer, then waits a preset amount of time for it to complete.
 * There's no feedback, and it can't be stopped once started, which makes things easy here.
 */


CStirrer::CStirrer(uint8_t pin)
{
  _pin = pin;
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);
  _last_used = millis();
  _state = CStirrer::IDLE;
  _pulse_sent = false;
}

CStirrer::~CStirrer()
{
  digitalWrite(_pin, HIGH);
}

uint8_t CStirrer::get_dispener_type()
{
  return DISPENSER_STIRRER;
}

bool CStirrer::dispense(uint8_t qty)
{
  if (_state != CStirrer::IDLE)
    return false;

  _state = CStirrer::BUSY;

  _dispense_start = millis();
  digitalWrite(_pin, LOW);
  _pulse_sent = false; // Well, it's been started...

  return true;
};

bool CStirrer::loop()
{
  if (_state != CStirrer::BUSY)
    return true;

  if ((!_pulse_sent) && millis()-_dispense_start >= 10) // 10 ms pulse
  {
     digitalWrite(_pin, HIGH);
     _pulse_sent = true;
  }
  else if (millis()-_dispense_start > STIRRER_TIME)
  {
    // Done!
    _state = CStirrer::IDLE;
  }

  return false;
}

void CStirrer::stop()
{
  // the stirrer can't be stopped once started
  _state = CStirrer::IDLE;
}

CDispenser::dispenser_state CStirrer::get_status()
{
  return _state;
}
