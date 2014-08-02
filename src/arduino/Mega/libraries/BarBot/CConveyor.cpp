#include "CConveyor.h"

/* Conveyor dispenser */


CConveyor::CConveyor(uint8_t start_stop, uint8_t end_sensor)
{
  _pin_start_stop = start_stop;
  _pin_end_sensor = end_sensor;

  pinMode(_pin_start_stop, OUTPUT);
  pinMode(_pin_end_sensor, INPUT_PULLUP);

  digitalWrite(_pin_start_stop, LOW);
  _last_used = millis();
  _state = CConveyor::IDLE;
  _cherry_sensed = 0;
}

CConveyor::~CConveyor()
{
  digitalWrite(_pin_start_stop, LOW);
}

uint8_t CConveyor::get_dispener_type()
{
  return DISPENSER_STIRRER;
}

bool CConveyor::dispense(uint8_t qty)
{
  if (_state != CConveyor::IDLE)
    return false;

  _state = CConveyor::BUSY;
  _cherry_sensed = 0;

  _dispense_start = millis();
  digitalWrite(_pin_start_stop, HIGH);

  return true;
};

bool CConveyor::loop()
{
  if (_state != CConveyor::BUSY)
    return true;

  // Look for a cherry being dispensed
  if ((_cherry_sensed == 0) && digitalRead(_pin_end_sensor) == HIGH)
    _cherry_sensed = millis();

  // Once a chery has been sensed, wait 1sec for the conveyor to reverse, then stop.
  if (_cherry_sensed && (millis()-_cherry_sensed) > CONVEYOR_REVERSE_TIME)
  {
    _state = CConveyor::IDLE;
    digitalWrite(_pin_start_stop, LOW);
  }

  // Add a timeout, in case no cherry is ever sensed
  if ((_cherry_sensed==0) && ((millis()-_dispense_start) > CONVEYOR_TIMEOUT))
  {
    _state = CConveyor::IDLE;
    digitalWrite(_pin_start_stop, LOW);
  }

  return false;
}

void CConveyor::stop()
{
  digitalWrite(_pin_start_stop, LOW);
  _state = CConveyor::IDLE;
}

CDispenser::dispenser_state CConveyor::get_status()
{
  return _state;
}
