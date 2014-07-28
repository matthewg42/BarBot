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
}

BarBot::~BarBot()
{
  for (int ix=1; ix < DISPENSER_COUNT; ix++)
    if (_dispeners[ix] != NULL)
      delete _dispeners[ix];
}

// Add an instruction to be carried out
bool BarBot::instruction_add(instruction_type instruction, uint16_t param1, uint16_t param2)
{
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
bool BarBot::go()
{
  
  return false;
}

// Needs to be called regulary whilst barbot is in action!
bool BarBot::loop()
{
  
  for (int ix=1; ix < DISPENSER_COUNT; ix++)
    if (_dispeners[ix] != NULL)
    {
      _dispeners[ix]->loop();
    }     
      
    
  
  return false;
}
    