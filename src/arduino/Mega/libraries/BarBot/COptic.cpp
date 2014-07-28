#include "COptic.h"



COptic::COptic(uint8_t servo_pin)
{
  _servo.attach(servo_pin);
  _servo.write(OPTIC_IDLE_POSITION);
  _last_used = millis();
  _state = COptic::IDLE;
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
  _servo.write(OPTIC_DISPENSE_POSITION);
  
  _dispense_start = millis();
  
  return false;  
};

bool COptic::loop()
{
  if (_state != COptic::BUSY)
    return true;
  
  if (millis()-_dispense_start >= OPTIC_DISPENSE_TIME)
  {
    _servo.write(OPTIC_IDLE_POSITION);
    _state != COptic::IDLE;
  }
  
  return true;
}
