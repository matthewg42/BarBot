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
        _dispeners[ix] = NULL;
        break;
        
      case 18:  // Slice dispenser
        _dispeners[ix] = NULL;
        break;
        
      case 19:  // Stirrer
        _dispeners[ix] = NULL;
        break;
        
      case 20:  // Umbrella
        _dispeners[ix] = NULL;
        break;
    }

  }
  
  // Stepper for platform movement
  _stepper = new AccelStepper(AccelStepper::DRIVER);
  _stepper->setMaxSpeed(1000);
  _stepper->setAcceleration(1000);
  _stepper->setPinsInverted(false,false,false,false,true);
  _stepper->setEnablePin(4);
  _stepper->disableOutputs();
  
  _state = BarBot::IDLE;
  _current_instruction = 0;
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
    
  _state = BarBot::RUNNING;
    
  
  return true;
}

bool BarBot::exec_instruction(uint16_t ins)
{
  instruction *cmd = &_instructions[ins];
  char buf[25]="";
  
  if (ins >= _instruction_count)
    return false;
  
  sprintf(buf, "exec ins[%d], typ[%d]", ins, cmd->type);
  debug(buf);
  
  switch (cmd->type)
  {
    case NOP:
      break;
      
    case MOVE:
      _move_start = millis();
      _stepper->moveTo(cmd->param1);
      break;
      
    case DISPENSE:
     _dispeners[cmd->param1]->dispense(cmd->param2); // nb. instruction_add validated that param1 was in bounds.
     break;   
     
    case WAIT:
      _wait_inst_start = millis();
      break;
  }
  
  return true;  
}

// Needs to be called regulary whilst barbot is in action!
bool BarBot::loop()
{
  instruction *cmd = &_instructions[_current_instruction];
  bool done = false;
  
  for (int ix=1; ix < DISPENSER_COUNT; ix++)
    if (_dispeners[ix] != NULL)
    {
      _dispeners[ix]->loop();
    }     
      
  _stepper->run();

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
          _stepper->disableOutputs();
          digitalWrite(13, LOW);            // TODO: 13?
          done = true;
        }
        if ((millis()-_move_start) > MAX_MOVE_TIME)
        {
          debug("Move timeout!");
          _stepper->disableOutputs();
          digitalWrite(13, LOW);            // TODO: 13?
          _state = BarBot::FAULT;
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
    }
        
    if (done)
    {
      if (!exec_instruction(++_current_instruction))
      {
        // exec_instruction returns false when there are no more instructions to execute.
        debug("Done! setting state=idle");
        _state = BarBot::IDLE;
      }
    }
  }
  
  return false;
}
   
void debug(char *msg)
{
  Serial.println(msg);
}