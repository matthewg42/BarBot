#include "COptic.h"



COptic::COptic(uint8_t servo_pin, uint8_t open_pos, uint8_t closed_pos)
{
  _servo.attach(servo_pin);
  _last_used = millis();
  _state = COptic::IDLE;
  _dispense_started = false;
  _closed_pos = closed_pos;
  _open_pos = open_pos;
  _servo.write(_closed_pos);
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
      _servo.write(_open_pos);
    }
  }
  else
  {
    if (millis()-_dispense_start >= OPTIC_DISPENSE_TIME)
    {
      _servo.write(_closed_pos);
      _last_used = millis();
      _state = COptic::IDLE;
    }
  }

  return true;
}

void COptic::stop()
{
  _servo.write(_closed_pos);
  _state = COptic::IDLE; 
}

CDispenser::dispenser_state COptic::get_status()
{
  return _state;
}
