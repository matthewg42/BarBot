#include "COptic.h"



COptic::COptic(uint8_t servo_pin)
{
  _servo.attach(servo_pin);
  _servo.write(OPTIC_IDLE_POSITION);
  _last_used = millis();
  _state = COptic::IDLE;
  _dispense_started = false;
}

COptic::~COptic()
{
  _servo.detach();
}

uint8_t COptic::get_dispener_type()
{
  return DISPENSER_OPTIC;
}

bool COptic::dispense(uint8_t qty)
{
  if (_state != COptic::IDLE)
    return false;
  
  _state = COptic::BUSY;
  _dispense_started = false;



  return false;
};

bool COptic::loop()
{
  if (_state != COptic::BUSY)
    return true;
  
  if (!_dispense_started)
  {
    // Ensure the optic has had time to refill since last use
    if (millis()-_last_used > OPTIC_RECHARGE_TIME)
    {
      _dispense_start = millis();
      _dispense_started = true;
      _servo.write(OPTIC_DISPENSE_POSITION);
    }
  }
  else
  {
    if (millis()-_dispense_start >= OPTIC_DISPENSE_TIME)
    {
      _servo.write(OPTIC_IDLE_POSITION);
      _last_used = millis();
      _state = COptic::IDLE;
    }
  }

  return true;
}

void COptic::stop()
{
  _servo.write(OPTIC_IDLE_POSITION);
  _state = COptic::IDLE; 
}

CDispenser::dispenser_state COptic::get_status()
{
  return _state;
}
