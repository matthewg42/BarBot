#include <AccelStepper.h>

#include <Servo.h>

#include <BarBot.h>

BarBot *bb;

void setup()
{
  Serial.begin(9600);
  Serial.println("Start!");
  bb = new BarBot();
  
 // bb->instruction_add(BarBot::ZERO, 0, 0);
  
  bb->instruction_add(BarBot::MOVE, 10, 0);    // optic0
  bb->instruction_add(BarBot::DISPENSE, 1, 1); 
  
  bb->instruction_add(BarBot::MOVE, 146, 1);     // Umberlla
  bb->instruction_add(BarBot::DISPENSE, 20, 0);   
  
  bb->instruction_add(BarBot::MOVE, 60, 1);    // optic5
  bb->instruction_add(BarBot::DISPENSE, 6, 1); 
  
  bb->instruction_add(BarBot::MOVE, 126, 1);     // Conveyor
  bb->instruction_add(BarBot::DISPENSE, 17, 0);  
  /*
  bb->instruction_add(BarBot::MOVE, 136, 1);     // Slice
  bb->instruction_add(BarBot::DISPENSE, 18, 0);  
  
  bb->instruction_add(BarBot::MOVE, 142, 1);     // Stirrer
  bb->instruction_add(BarBot::DISPENSE, 19, 0);    
   */
 
  bb->instruction_add(BarBot::MOVE, 0, 1);     // return to home
  
  
  
  bb->go();
}

void loop()
{
  
  bb->loop();
  
}
