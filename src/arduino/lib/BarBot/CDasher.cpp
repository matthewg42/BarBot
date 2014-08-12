#include "CDasher.h"

/* Dasher dispenser */


CDasher::CDasher(uint8_t cam, uint8_t driver)
{
  _pin_cam = cam;
  _pin_driver = driver;
  
  pinMode(_pin_driver, OUTPUT);
  pinMode(_pin_cam, INPUT_PULLUP);
  
  digitalWrite(_pin_driver, LOW);
  
  _last_used = millis();
  _state = CDasher::IDLE;
}

CDasher::~CDasher()
{
  digitalWrite(_pin_driver, LOW);
}

uint8_t CDasher::get_dispener_type()
{
  return DISPENSER_DASHER;
}

bool CDasher::dispense(uint16_t qty)
{
  if (_state != CDasher::IDLE)
    return false;

  _state = CDasher::BUSY;
  _qty = qty+1;

  _dispense_start = millis();
  digitalWrite(_pin_driver, HIGH);
  
  return true;
};

bool CDasher::loop()
{
  static uint8_t last_cam_val = LOW;

  if (_state != CDasher::BUSY)
    return true;
  
  // Count number of dashes by looking for pulses on cam line
  uint8_t cam_val = digitalRead(_pin_cam);
  if (cam_val != last_cam_val)
  {
    _qty--;
    last_cam_val = cam_val;
  }
  
  // If we've dispensed the required number of dashes, then stop. 
  if (_qty <= 0)
  {
    digitalWrite(_pin_driver, LOW);
    _state = CDasher::IDLE;
    return false;
  }    

  if (millis()-_dispense_start > DASHER_TIMEOUT) 
  {
     digitalWrite(_pin_driver, LOW);
     _state = CDasher::IDLE;
     debug("Dasher timeout!");
     return false;
  }

  return false;
}

void CDasher::stop()
{
  digitalWrite(_pin_driver, LOW);
  _state = CDasher::IDLE;
}

CDispenser::dispenser_state CDasher::get_status()
{
  return _state;
}
