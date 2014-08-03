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
      case 1: _dispeners[ix] = new COptic(40); break; // Optic0
      case 2: _dispeners[ix] = new COptic(42); break; // Optic1
      case 3: _dispeners[ix] = new COptic(44); break; // Optic2
      case 4: _dispeners[ix] = new COptic(46); break; // Optic3
      case 5: _dispeners[ix] = new COptic(48); break; // Optic4
      case 6: _dispeners[ix] = new COptic(50); break; // Optic5

      case 7:   // Preasure0
      case 8:   // Preasure1
      case 9:   // Preasure2 
      case 10:  // Preasure3
      case 11:  // Preasure4
      case 12:  // Preasure5
        // TODO: preasure type
        //dispeners[ix] = new CPreasure();
        _dispeners[ix] = NULL;
        break;
        
      case 13:  // Dasher0
      case 14:  // Dasher1
      case 15:  // Dasher2
        // TODO
        _dispeners[ix] = NULL;
        break;
        
      case 16:  // Syringe
        _dispeners[ix] = NULL;
        break;
        
      case 17:  // Conveyor
        _dispeners[ix] = new CConveyor(38, 39);
        break;
        
      case 18:  // Slice dispenser
        _dispeners[ix] = new CSlice(34);
        break;
        
      case 19:  // Stirrer
        _dispeners[ix] = new CStirrer(36);
        break;
        
      case 20:  // Umbrella
        _dispeners[ix] = new CUmbrella(32);
        break;
    }

  }
  
  // Stepper for platform movement
  _stepper = new AccelStepper(AccelStepper::DRIVER);
  _stepper->setMaxSpeed(SPEED_NORMAL);
  _stepper->setAcceleration(MAX_ACCEL);
  _stepper->setPinsInverted(false,false,false,false,true);
  _stepper->setEnablePin(4);
  //_stepper->disableOutputs();
  
  set_state(BarBot::IDLE);
  _current_instruction = 0;
  _stepper_target = 0;
  
  pinMode(ZERO_SWITCH, INPUT_PULLUP);
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
  memset(_instructions, NOP, sizeof(_instructions));  
  _instruction_count = 0;
  return true;
}

// Make the drink!
// Returns true if drink making has started, false otherwise
bool BarBot::go()
{  
  if (_state != BarBot::IDLE)
    return false;
  
  if (_instruction_count <= 0)
    return false;
  
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
      move_to((cmd->param1 * STEPS_PER_CM));
      _stepper->run();
      break;

    case DISPENSE:
     _dispeners[cmd->param1]->dispense(cmd->param2); // nb. instruction_add validated that param1 was in bounds.
     break;

    case WAIT:
      _wait_inst_start = millis();
      break;

    case ZERO:
      _stepper->setMaxSpeed(SPEED_ZERO);
      move_to(-500);  // TODO: suitable value
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
        if (_dispeners[cmd->param1]->get_status() == CDispenser::IDLE)
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

void BarBot::set_state(barbot_state state)
{
  _state = state;
  
  if (_state == BarBot::FAULT)
  {
    debug("FAULT.");

    // Stop platform
    _stepper->stop();

    // Stop all dispensers
    for (int ix=1; ix < DISPENSER_COUNT; ix++)
      if (_dispeners[ix] != NULL)
        _dispeners[ix]->stop();
  }
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
   
void debug(char *msg)
{
  Serial.println(msg);
}