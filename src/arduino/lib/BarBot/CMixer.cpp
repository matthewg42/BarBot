#include "CMixer.h"

/* mixer dispenser - qty is ml to dispense */

CMixer::CMixer(uint8_t servo_pin)
{
  _servo.attach(servo_pin);
  _servo.write(MIXER_IDLE_POSITION);
  _last_used = millis();
  _state = CMixer::IDLE;
}

CMixer::~CMixer()
{
  _servo.detach();
}

uint8_t CMixer::get_dispener_type()
{
  return DISPENSER_MIXER;
}

bool CMixer::dispense(uint8_t qty)
{
  if (_state != CMixer::IDLE)
    return false;

  _state = CMixer::BUSY;
  _dispense_start = millis();
  _servo.write(MIXER_DISPENSE_POSITION);
  _dispense_time = (qty * ML_PER_MS);

  return false;
};

bool CMixer::loop()
{
  if (_state != CMixer::BUSY)
    return true;

  if (millis()-_dispense_start >= _dispense_time)
  {
    _servo.write(MIXER_IDLE_POSITION);
    _last_used = millis();
    _state = CMixer::IDLE;
  }

  return true;
}

void CMixer::stop()
{
  _servo.write(MIXER_IDLE_POSITION);
  _state = CMixer::IDLE; 
}

CDispenser::dispenser_state CMixer::get_status()
{
  return _state;
}
