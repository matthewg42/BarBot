#include "CStirrer.h"

/* Stirrer dispenser
 * Note: Dispensing is regarded as complete as soon as a ~10ms pulse is sent. I.e. there
 * is an assumption that that a WAIT insturction of suitable length will follow
 */


CStirrer::CStirrer(uint8_t pin)
{
  _pin = pin;
  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, HIGH);
  _last_used = millis();
  _state = CStirrer::IDLE;
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

  return true;
};

bool CStirrer::loop()
{
  if (_state != CStirrer::BUSY)
    return true;

  if (millis()-_dispense_start >= 10) // 10 ms pulse
  {
    _state = CStirrer::IDLE;
     digitalWrite(_pin, HIGH);
  }

  return false;
}

void CStirrer::stop()
{

  _state = CStirrer::IDLE;
}

CDispenser::dispenser_state CStirrer::get_status()
{
  return _state;
}
