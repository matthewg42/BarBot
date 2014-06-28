
// Nottinghack Barbot Platform
// ATtiny85 (Needs 8 MHz clock for Neopixel library)
// V0.1
// Ultrasonic glass detection and Neopixel illumination

// NeoPixel ring
#include <Adafruit_NeoPixel.h>
#define NEO_PIN 0

// Ultrasonic distance sensor 
#define TRIG_PIN  1
#define ECHO_PIN  2

// Glass distance range 
#define MIN_GLASS_CM 2
#define MAX_GLASS_CM 5

// Glass present output
#define GP_PIN 3

typedef enum {RED,AMBER,GREEN,READY} glassState_t;
glassState_t glassState=RED;

long timeOut;
#define TIMEOUT 2500

Adafruit_NeoPixel strip = Adafruit_NeoPixel(24,NEO_PIN,NEO_GRB+NEO_KHZ800);

void setup() 
{
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);

  pinMode(GP_PIN,OUTPUT);
  digitalWrite(GP_PIN,LOW);

  strip.begin();
  strip.show();
}

void loop() 
{
  int d=readDistance();
  boolean glassPresent=(d>=MIN_GLASS_CM && d<=MAX_GLASS_CM);
  
  switch(glassState)
  {
    case RED:
      if(glassPresent)
      {
        glassState=AMBER;
        timeOut=millis()+TIMEOUT;
      } 
      else
      {
        colorWipe(strip.Color(255,0,0),10); // red
        colorWipe(strip.Color(0,0,0),10);   // blank
      }
      break;
     
    case AMBER:
      if(glassPresent)
      {
        if(millis()>timeOut)
        {
          glassState=GREEN;
          timeOut=millis()+TIMEOUT;
        }
        else
        {
          colorWipe(strip.Color(255,80,0),10); // amber
          colorWipe(strip.Color(0,0,0),10);     // blank
        }
      }
      else
      {
        digitalWrite(GP_PIN,LOW);
        glassState=RED;        
      }
      break;

    case GREEN:
      if(glassPresent)
      {
        if(millis()>timeOut)
        {
          glassState=READY;
          digitalWrite(GP_PIN,HIGH);
        }
        else
        {
          colorWipe(strip.Color(0,255,0),10); // green
          colorWipe(strip.Color(0,0,0),10);   // blank
        }
      }
      else
      {
        digitalWrite(GP_PIN,LOW);
        glassState=RED;        
      }
      break;
     
    case READY:
      if(glassPresent)
      {
        rainbowCycle(10);
      }
      else
      {
        digitalWrite(GP_PIN,LOW);
        glassState=RED;        
      }
      break;
     
    default:
      digitalWrite(GP_PIN,LOW);
      glassState=RED;
  }

  delay(100);
}

void colorWipe(uint32_t c, uint8_t wait) 
{
  for(uint16_t i=0; i<strip.numPixels(); i++) 
  {
      strip.setPixelColor(i, c);
      strip.show();
      delay(wait);
  }
}

void rainbowCycle(uint8_t wait) 
{
  uint16_t i, j;

  for(j=0; j<256; j++) 
  {
    for(i=0; i< strip.numPixels(); i++) 
    {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) 
{
  if(WheelPos < 85) 
  {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) 
  {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else 
  {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

int readDistance()
{
  long d;
  
  digitalWrite(TRIG_PIN,LOW); 
  delayMicroseconds(2); 

  digitalWrite(TRIG_PIN,HIGH);
  delayMicroseconds(10); 
 
  digitalWrite(TRIG_PIN,LOW);

  //Calculate the distance (in cm) based on the speed of sound.
  d=pulseIn(ECHO_PIN,HIGH,10000)/58.2;

  return d;
} 

