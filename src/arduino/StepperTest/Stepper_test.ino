
// approx 55 steps / cm

#include <AccelStepper.h>
AccelStepper stepper(AccelStepper::DRIVER);

#include <Adafruit_NeoPixel.h>
#define NEO0_PIN 7
#define NEO1_PIN 8
#define NEO2_PIN 9
#define NEO3_PIN 10
#define NEO4_PIN 11
#define NEO5_PIN 12

Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(72,NEO0_PIN,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(24,NEO1_PIN,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(24,NEO2_PIN,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(24,NEO3_PIN,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(24,NEO4_PIN,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(24,NEO5_PIN,NEO_GRB+NEO_KHZ800);

#include <Servo.h> 
Servo servoOptic0; 
Servo servoOptic1; 
Servo servoOptic2; 
Servo servoOptic3; 
Servo servoOptic4; 
Servo servoOptic5; 

Servo servoMixer0; 
Servo servoMixer1; 
Servo servoMixer2; 
Servo servoMixer3; 
Servo servoMixer4; 
Servo servoMixer5; 

void colorWipe(uint32_t c, uint8_t wait);
void rainbowCycle(uint8_t wait);
uint32_t Wheel(byte WheelPos);

void setup()
{  
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(1000);
  stepper.setPinsInverted(false,false,false,false,true);
  stepper.setEnablePin(4);
  stepper.disableOutputs();

  pinMode(13,OUTPUT);
 
  strip0.begin();
  strip1.begin();
  strip2.begin();
  strip3.begin();
  strip4.begin();
  strip5.begin();

  strip0.show();
  strip1.show();
  strip2.show();
  strip3.show();
  strip4.show();
  strip4.show();
 
  servoOptic0.attach(40);
  servoOptic1.attach(42);
  servoOptic2.attach(44);
  servoOptic3.attach(46);
  servoOptic4.attach(48);
  servoOptic5.attach(50);

  servoMixer0.attach(41);
  servoMixer1.attach(43);
  servoMixer2.attach(45);
  servoMixer3.attach(47);
  servoMixer4.attach(49);
  servoMixer5.attach(51);

  pinMode(32,OUTPUT); // Umbrella
  digitalWrite(32,LOW);
  
  pinMode(34,OUTPUT); // Lemon 
  digitalWrite(34,HIGH);

  pinMode(36,OUTPUT); // Stir
  digitalWrite(36,HIGH);

  pinMode(38,OUTPUT); // Conveyor
  pinMode(39,INPUT_PULLUP);
  digitalWrite(38,HIGH);
  
  pinMode(22,INPUT_PULLUP); // Dasher 0
  pinMode(23,OUTPUT); 
  digitalWrite(23,LOW);
  
  pinMode(24,INPUT_PULLUP); // Dasher 1
  pinMode(25,OUTPUT); 
  digitalWrite(25,LOW);

  pinMode(26,INPUT_PULLUP); // Dasher 2
  pinMode(27,OUTPUT); 
  digitalWrite(27,LOW);

  pinMode(37,INPUT_PULLUP);  // zero switch
  pinMode(53,INPUT_PULLUP);  // emergency stop switch
  
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

// Stepper tests
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
      stepper.moveTo(5000);
      break;

    case '7':
      stepper.moveTo(6500);
      break;

    case '8':
      stepper.moveTo(6800);
      break;

    case '9':
      stepper.moveTo(7000);
      break;

// Neopixel tests
    case 'A':
    case 'a':
     colorWipe(strip0.Color(255,0,0),10); // red
     break;

    case 'B':
    case 'b':
     colorWipe(strip0.Color(0,255,0),10); // green
     break;

    case 'C':
    case 'c':
     colorWipe(strip0.Color(0,0,255),10); // blue
     break;
     
    case 'D':
    case 'd':
     colorWipe(strip0.Color(255,255,255),10); // white
     break;

    case 'E':
    case 'e':
     rainbowCycle(5);
     break;

    case 'F':
    case 'f':
     colorWipe(strip0.Color(0,0,0),10); // off
     break;

// Servo tests     
    case 'G':
    case 'g':
      servoOptic0.write(10);
      servoOptic1.write(10);
      servoOptic2.write(10);
      servoOptic3.write(10);
      servoOptic4.write(10);
      servoOptic5.write(10);

      servoMixer0.write(10);
      servoMixer1.write(10);
      servoMixer2.write(10);
      servoMixer3.write(10);
      servoMixer4.write(10);
      servoMixer5.write(10);
      break;

    case 'H':
    case 'h':
      servoOptic0.write(70);
      servoOptic1.write(70);
      servoOptic2.write(70);
      servoOptic3.write(70);
      servoOptic4.write(70);
      servoOptic5.write(70);

      servoMixer0.write(70);
      servoMixer1.write(70);
      servoMixer2.write(70);
      servoMixer3.write(70);
      servoMixer4.write(70);
      servoMixer5.write(70);
      break;
      
// Big syringe H-bridge tests
    case 'I':
    case 'i':
      analogWrite(5,0);
      analogWrite(6,0);
      break;
      
    case 'J':   // this code sucks!
    case 'j':
      analogWrite(5,150);
      analogWrite(6,0);
      delay(500);
      analogWrite(5,0);
      analogWrite(6,0);
      break;

    case 'K':  // this code dispenses, but then sucks a little
    case 'k':
      analogWrite(5,0);
      analogWrite(6,150);
      delay(200);
      analogWrite(5,0);
      analogWrite(6,0);
      analogWrite(5,150);
      analogWrite(6,0);
      delay(100);
      analogWrite(5,0);
      analogWrite(6,0);
      break;

// Umbrella
    case 'L':
    case 'l':
      digitalWrite(32,HIGH);
      delay(1000);
      digitalWrite(32,LOW);
      break;

// Lemon
    case 'M':
    case 'm':
      digitalWrite(34,LOW);
      delay(10);
      digitalWrite(34,HIGH);
      break;

// Stir
    case 'N':
    case 'n':
      digitalWrite(36,LOW);
      delay(10);
      digitalWrite(36,HIGH);
      break;

// Conveyor
    case 'O':   // conveyor start
      digitalWrite(38,LOW);
      delay(10);
      digitalWrite(38,HIGH);
      break;

    case 'o':  // conveyor stop
      digitalWrite(38,LOW);
      break;

    case 'S':  // conveyor sensor
    case 's':
      Serial.println(digitalRead(39));
      break;

// Dasher 0
    case 'P':
    case 'p':
      digitalWrite(23,HIGH);
      while(!digitalRead(22));
      while(digitalRead(22));
      while(!digitalRead(22));
      while(digitalRead(22));
      digitalWrite(23,LOW);
      break;

// Dasher 1
    case 'Q':
    case 'q':
      digitalWrite(25,HIGH);
      while(!digitalRead(24));
      while(digitalRead(24));
      while(!digitalRead(24));
      while(digitalRead(24));
      digitalWrite(25,LOW);
      break;

// Dasher 2
    case 'R':
    case 'r':
      digitalWrite(27,HIGH);
      while(!digitalRead(26));
      while(digitalRead(26));
      while(!digitalRead(26));
      while(digitalRead(26));
      digitalWrite(27,LOW);
      break;

    case 'T':  // zero switch
    case 't':
      Serial.println(digitalRead(37));
      break;

    case 'U':  // emergency stop
    case 'u':
      Serial.println(digitalRead(53));
      break;

    default:
      break;
  }
}

void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip0.numPixels(); i++) 
    strip0.setPixelColor(i, c);

  strip0.show();
}

void rainbowCycle(uint8_t wait) 
{
  uint16_t i, j;

  for(i=0; i< strip0.numPixels(); i++) 
    strip0.setPixelColor(i, Wheel((i * 256 / strip0.numPixels()) & 255));

  strip0.show();
}

uint32_t Wheel(byte WheelPos) 
{
  if(WheelPos < 85) 
  {
    return strip0.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) 
  {
    WheelPos -= 85;
    return strip0.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else 
  {
    WheelPos -= 170;
    return strip0.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

