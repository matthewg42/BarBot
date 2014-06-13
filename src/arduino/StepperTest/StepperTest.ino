
// approx 55 steps / cm

#include <AccelStepper.h>

AccelStepper stepper(AccelStepper::DRIVER);

void setup()
{  
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(1000);
  stepper.setPinsInverted(false,false,false,false,true);
  stepper.setEnablePin(4);
  stepper.disableOutputs();

  pinMode(13,OUTPUT);
  
  Serial.begin(9600);
}

void loop()
{
  if (stepper.distanceToGo() == 0)
  {
    stepper.disableOutputs();
    digitalWrite(13,LOW);
  }
  
  stepper.run();
}

void serialEvent()
{
  char c;
  
  c=(char)Serial.read();
  Serial.println(c);
  
  stepper.enableOutputs();
  digitalWrite(13,HIGH);
  
  switch(c)
  {
    case '0':
      stepper.moveTo(0);
      break;

    case '1':
      stepper.moveTo(500);
      break;
  
    case '2':
      stepper.moveTo(1000);
      break;

    case '3':
      stepper.moveTo(1500);
      break;

    case '4':
      stepper.moveTo(2000);
      break;

    case '5':
      stepper.moveTo(2500);
      break;

    case '6':
      stepper.moveTo(3000);
      break;

    case '9':
      stepper.moveTo(7000);
      break;
  
    default:
      break;    
  }
}

