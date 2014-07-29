#include <Servo.h>
#include <BarBot.h>

BarBot *bb;

void setup()
{
  Serial.begin(9600);
  Serial.println("Start!");
  bb = new BarBot();
  
//bb.instruction_add(BarBot::MOVE, 100, -1); 
  bb->instruction_add(BarBot::DISPENSE, 1, 1); 
  bb->instruction_add(BarBot::WAIT, 2000, 1); 
  bb->instruction_add(BarBot::DISPENSE, 2, 1); 
  bb->go();
}

void loop()
{
  
  bb->loop();
  
}
