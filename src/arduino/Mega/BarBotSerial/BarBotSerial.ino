#include <Servo.h>
#include <BarBot.h>

BarBot bb;

void setup()
{
  bb.instruction_add(BarBot::MOVE, 100, -1); 
  bb.instruction_add(BarBot::DISPENSE, 1, 1); 
}

void loop()
{
  
  bb.loop();
  
}
