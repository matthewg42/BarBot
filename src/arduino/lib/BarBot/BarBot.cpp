#include "BarBot.h"

BarBot::BarBot()
{
  memset(_instructions, NOP, sizeof(_instructions));
  _instruction_count = 0;
  
  // Note: deliberately skipping ix=0 so array index matches dispenser.id in the database 
  for (int ix=1; ix < DISPENSER_COUNT; ix++)
  {
    switch(ix)
    {
      case 1:  _dispeners[ix] = new COptic(40, 65, 10); break; // Optic0
      case 2:  _dispeners[ix] = new COptic(42, 10, 65); break; // Optic1
      case 3:  _dispeners[ix] = new COptic(44, 65, 10); break; // Optic2
      case 4:  _dispeners[ix] = new COptic(46, 10, 65); break; // Optic3
      case 5:  _dispeners[ix] = new COptic(48, 65, 10); break; // Optic4
      case 6:  _dispeners[ix] = new COptic(50, 65, 10); break; // Optic5

      case 7:  _dispeners[ix] = new CMixer(41); break; // Preasure0
      case 8:  _dispeners[ix] = new CMixer(43); break; // Preasure1
      case 9:  _dispeners[ix] = new CMixer(45); break; // Preasure2 
      case 10: _dispeners[ix] = new CMixer(47); break; // Preasure3
      case 11: _dispeners[ix] = new CMixer(49); break; // Preasure4
      case 12: _dispeners[ix] = new CMixer(51); break; // Preasure5
        
      case 13:  _dispeners[ix] = new CDasher(22, 23);  break; // Dasher0
      case 14:  _dispeners[ix] = new CDasher(24, 25);  break; // Dasher1
      case 15:  _dispeners[ix] = new CDasher(26, 27);  break; // Dasher2
        
      case 16: /* TODO */   _dispeners[ix] = NULL;     break;  // Syringe
        
      case 17: _dispeners[ix] = new CConveyor(38, 39); break;  // Conveyor
        
      case 18: _dispeners[ix] = new CSlice(34);        break;  // Slice dispenser
        
      case 19: _dispeners[ix] = new CStirrer(36);      break;  // Stirrer
        
      case 20: _dispeners[ix] = new CUmbrella(32);     break;  // Umbrella
    }
  }
  
  // Stepper for platform movement
  _stepper = new AccelStepper(AccelStepper::DRIVER);
  _stepper->setMaxSpeed(SPEED_NORMAL);
  _stepper->setAcceleration(MAX_ACCEL);
  _stepper->setPinsInverted(false,false,false,false,true);
  _stepper->setEnablePin(4);
  _stepper->disableOutputs();
  _stepper->run();
  
  set_state(BarBot::IDLE);
  _current_instruction = 0;
  _stepper_target = 0;
  
  pinMode(ZERO_SWITCH, INPUT_PULLUP);
  pinMode(ESTOP_PIN  , INPUT_PULLUP);
}

BarBot::~BarBot()
{
  for (int ix=1; ix < DISPENSER_COUNT; ix++)
    if (_dispeners[ix] != NULL)
      delete _dispeners[ix];
}

// Add an instruction to be carried out
// Returns: true if instruction added, false otherwise
bool BarBot::instruction_add(instruction_type instruction, uint16_t param1, uint16_t param2)
{
  if (_state == BarBot::RUNNING)
  {
    debug("Error: barbot running, can't add");
    return false;
  } 

  // For DISPENSE instructions, param1 is the dispenser_id - ensure this is valid.
  if ((instruction == BarBot::DISPENSE) && (param1 >= DISPENSER_COUNT))
    return false;    
  
  if (_instruction_count < MAX_INSTRUCTIONS)
  {
    _instructions[_instruction_count].type   = instruction;
    _instructions[_instruction_count].param1 = param1;
    _instructions[_instruction_count].param2 = param2;
    _instruction_count++;
    return true;
  }
  else
  {
    return false;
  }
}

// Clear the insturction list
bool BarBot::instructions_clear()
{
  if (_state != BarBot::RUNNING)
  {
    memset(_instructions, NOP, sizeof(_instructions));  
    _instruction_count = 0;
    return true;
  }
}

bool BarBot::reset()
{
  set_state(BarBot::FAULT); // Ensure stopped
  instructions_clear();
  move_to(0);
  set_state(BarBot::IDLE);
}

// Make the drink!
// Returns true if drink making has started, false otherwise
bool BarBot::go()
{  
  if (_state != BarBot::IDLE)
  {
    debug("go failed: not idle");
    return false;
  }
  
  if (_instruction_count <= 0)
  {
    debug("go failed: no instructions");
    return false;
  }
  
  _current_instruction = 0;
  
  
  exec_instruction(_current_instruction);
    
  set_state(BarBot::RUNNING);
    
  
  return true;
}


bool BarBot::exec_instruction(uint16_t ins)
{
  instruction *cmd = &_instructions[ins];
  char buf[40]="";

  if (ins >= _instruction_count)
    return false;

  sprintf(buf, "exec ins[%d], typ[%d], p1[%d] p2[%d]", ins, cmd->type, cmd->param1, cmd->param2);
  debug(buf);

  switch (cmd->type)
  {
    case NOP:
      break;

    case MOVE:
      move_to(cmd->param1);
      _stepper->run();
      break;

    case DISPENSE:
      if (_dispeners[cmd->param1] != NULL)
        _dispeners[cmd->param1]->dispense(cmd->param2); // nb. instruction_add validated that param1 was in bounds.
     break;

    case WAIT:
      _wait_inst_start = millis();
      break;

    case ZERO:
      _stepper->setMaxSpeed(SPEED_ZERO);
      move_to(-7300);  // TODO: suitable value
      _stepper->run();
      break;
  }

  return true;  
}

// Needs to be called regulary whilst barbot is in action!
bool BarBot::loop()
{
  instruction *cmd = &_instructions[_current_instruction];
  bool done = false;
  
  _stepper->run();
    
  for (int ix=1; ix < DISPENSER_COUNT; ix++)
    if (_dispeners[ix] != NULL)
    {
      _dispeners[ix]->loop();
    }

  _stepper->run();
  
  // Double check - if the limit switch is ever hit, always stop the platform
  if 
  (
    (digitalRead(ZERO_SWITCH) == LOW) &&   // Limit switch hit
    (_stepper_target > 0) &&               // We're not aiming for it
    (millis()-_move_start > 250) &&        // Current move has been in progress for a while (i.e. plenty of time to have moved off the limit switch)
    (_state != BarBot::FAULT)              // We've not already faulted.
  )
  {
    debug("Error: limit switch unexpectedly hit!");
    set_state(BarBot::FAULT);
  }
  
  _stepper->run();
  
  // Look for Emergency stop button being pressed
  if ((_state != BarBot::FAULT) && (digitalRead(ESTOP_PIN) == HIGH))
  {
    debug("ESTOP");
    set_state(BarBot::FAULT);
  }

  // If running, find out if the last executed insturction has finished 
  if (_state == BarBot::RUNNING)
  {
    switch (cmd->type)
    {
      case NOP:
        done = true;
        break;
        
      case MOVE:
        if (_stepper->distanceToGo() == 0)
        {
          //_stepper->disableOutputs();
          done = true;
        }
        if ((millis()-_move_start) > MAX_MOVE_TIME)
        {
          debug("Move timeout!");
          set_state(BarBot::FAULT);
        } 
        break;
        
      case DISPENSE:
        if (_dispeners[cmd->param1] != NULL)
        {
          if (_dispeners[cmd->param1]->get_status() == CDispenser::IDLE)
            done = true;
        } else
          done = true;
        break;
        
      case WAIT:
        if ((millis()-_wait_inst_start) >= cmd->param1)
          done = true;
        break;
        
      case ZERO:
        if (digitalRead(ZERO_SWITCH) == LOW)
        {
          done = true;
          _stepper->stop();
          _stepper->setCurrentPosition(0);
          // _stepper->disableOutputs();
          _stepper->setMaxSpeed(SPEED_NORMAL);
        } 
        else if (_stepper->distanceToGo() == 0)
        {
          debug("FAULT: distanceToGo=0 whilst zeroing!");
          set_state(BarBot::FAULT);
          _stepper->setMaxSpeed(SPEED_NORMAL);
        }
        else if (millis()-_move_start > MAX_MOVE_TIME)
        {
          debug("FAULT: ZERO timeout");
          set_state(BarBot::FAULT);
          _stepper->setMaxSpeed(SPEED_NORMAL);
        }
        break;
    }
  
    _stepper->run();
  
    if (done)
    {
      if (!exec_instruction(++_current_instruction))
      {
        // exec_instruction returns false when there are no more instructions to execute.
        debug("Done! setting state=idle");
        _stepper->disableOutputs();
        set_state(BarBot::IDLE);
      }
    }
  }
  
  return false;
}

void BarBot::set_state(barbot_state new_state)
{  
  if (new_state == BarBot::FAULT)
  {
    debug("FAULT.");

    // Stop platform
    _stepper->stop();

    _stepper->run();
    
    // Stop all dispensers
    for (int ix=1; ix < DISPENSER_COUNT; ix++)
      if (_dispeners[ix] != NULL)
        _dispeners[ix]->stop();
    _stepper->disableOutputs();
  }
  
  // Don't allow leaving FAULT state if emergency stop pressed
  if 
  (
    (_state == BarBot::FAULT)    && 
    (new_state != BarBot::FAULT) &&
    (digitalRead(ESTOP_PIN) == HIGH)
  )
  {
    debug("ESTOP ACTIVE");
    return;
  }   
  
  _state = new_state;  
}

void BarBot::move_to(long pos)
{
  char buf[30]="";
  if (pos > MAX_RAIL_POSITION)
  {
    pos = MAX_RAIL_POSITION;
    debug("Excessive rail position");
  }
  _stepper->enableOutputs();
  _stepper_target = pos;
  _stepper->moveTo(pos);
  _move_start = millis();
  sprintf(buf, "move=%d", pos);
  debug(buf);
}

BarBot::barbot_state BarBot::get_state()
{
  return _state;
}
   
void debug(char *msg)
{
  Serial.println(msg);
}