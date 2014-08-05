
// approx 55 steps / cm

#include <AccelStepper.h>
#include <Adafruit_NeoPixel.h>
#include <Servo.h> 

// Function prototypes
void colorWipe(uint32_t c, uint8_t wait);
void rainbowCycle(uint8_t wait);
uint32_t Wheel(byte WheelPos);
void SetOptic(uint8_t id, bool posOpen);

// Neopixel setup
#define NEO0_PIN 7
#define NEO1_PIN 8
#define NEO2_PIN 9
#define NEO3_PIN 10
#define NEO4_PIN 11
#define NEO5_PIN 12

#define ZERO_SWITCH_PIN 52
#define EMERGENCY_STOP_PIN 53

uint8_t opticServoPin[]       = { 40, 42, 44, 46, 48, 50 };
uint8_t opticServoClosedPos[] = { 10, 10, 10, 65, 10, 10 };
uint8_t opticServoOpenPos[]   = { 65, 65, 65, 10, 65, 65 };

// Some global objects which we will use
Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(24,NEO0_PIN,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(24,NEO1_PIN,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(24,NEO2_PIN,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel strip3 = Adafruit_NeoPixel(24,NEO3_PIN,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel strip4 = Adafruit_NeoPixel(24,NEO4_PIN,NEO_GRB+NEO_KHZ800);
Adafruit_NeoPixel strip5 = Adafruit_NeoPixel(24,NEO5_PIN,NEO_GRB+NEO_KHZ800);

AccelStepper stepper(AccelStepper::DRIVER);

Servo servoOptic[6];

Servo servoMixer0; 
Servo servoMixer1; 
Servo servoMixer2; 
Servo servoMixer3; 
Servo servoMixer4; 
Servo servoMixer5; 

void setup()
{  
    stepper.setMaxSpeed(1500);
    stepper.setAcceleration(3000);
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
 
    for(uint8_t i=0; i<6; i++) {
        servoOptic[i].attach(opticServoPin[i]);  // set the pin
        SetOptic(i, false);                      // make sure it's closed
    }

    // Make sure all the optics are closed
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

    pinMode(52,INPUT_PULLUP);  // zero switch
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
    case '0': // optic 0
      stepper.moveTo(0);
      break;

    case '1': // optic 1
      stepper.moveTo(500);
      break;
  
    case '2': // optic 2
      stepper.moveTo(1100);
      break;

    case '3': // optic 3
      stepper.moveTo(1650);
      break;

    case '4': // optic 4
      stepper.moveTo(2150);
      break;

    case '5': // optic 5
      stepper.moveTo(2680);
      break;

    case '6': // cherries
      stepper.moveTo(6000);
      break;

    case '7': // lemons
      stepper.moveTo(6500);
      break;

    case '8': // stirrer
      stepper.moveTo(6800);
      break;

    case '9': // umberlla
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

    // Close optics
    case 'G':
    case 'g':
        for (int i=0; i<6; i++) {
            SetOptic(i, false);
            delay(100);
        }
        break;

    // Open optics
    case 'H':
    case 'h':
        for (int i=0; i<6; i++) {
            SetOptic(i, true);
            delay(100);
        }
        break;
      
// Big syringe H-bridge tests
    case 'I':
    case 'i':
      analogWrite(5,0);
      analogWrite(6,0);
      break;
      
    case 'J':
    case 'j':
      analogWrite(5,150);
      analogWrite(6,0);
      delay(500);
      analogWrite(5,0);
      analogWrite(6,0);
      break;

    case 'K':
    case 'k':
      analogWrite(5,0);
      analogWrite(6,150);
      delay(500);
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

// Stirrer
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
      Serial.println(digitalRead(ZERO_SWITCH_PIN));
      break;

    case 'U':  // emergency stop
    case 'u':
      Serial.println(digitalRead(EMERGENCY_STOP_PIN));
      break;

    default:
      break;
  }
}

void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip0.numPixels(); i++) 
  {
    strip0.setPixelColor(i, c);
    strip0.show();
    delay(wait);
  }
}

void rainbowCycle(uint8_t wait) 
{
  uint16_t i, j;

  for(j=0; j<256; j++) 
  {
    for(i=0; i< strip0.numPixels(); i++) 
    {
      strip0.setPixelColor(i, Wheel(((i * 256 / strip0.numPixels()) + j) & 255));
    }
    strip0.show();
    delay(wait);
  }
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

void SetOptic(uint8_t id, bool posOpen) {
    if (posOpen) {
        servoOptic[id].write(opticServoOpenPos[id]);
    }
    else {
        servoOptic[id].write(opticServoClosedPos[id]);
    }
}

