
// Nottinghack Barbot Lemon Slice Vendor
// ATtiny85 - 1MHz
// V0.1
// V0.2 - now uses TRIG input to start

// L298N dual H bridge inputs
#define IN1 4
#define IN2 0

#define TRIG 1
#define CAM_SW 3

#define TRIG_TIME 5  // trigger active low time

void setup()
{
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(TRIG,INPUT_PULLUP);

  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
}

void loop()
{
  long end_time;

  while(digitalRead(TRIG)); 

  end_time=millis()+TRIG_TIME;

  while(!digitalRead(TRIG));

  if(millis()>end_time)
  {
    pinMode(CAM_SW,INPUT_PULLUP);

    digitalWrite(IN1,HIGH);
    delay(50);

    while(!digitalRead(CAM_SW));
    delay(50);

    while(digitalRead(CAM_SW));
    delay(50);

    digitalWrite(IN1,LOW);
  }
}


