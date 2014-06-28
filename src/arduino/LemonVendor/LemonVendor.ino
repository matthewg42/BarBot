
// Nottinghack Barbot Lemon Slice Vendor
// ATtiny85
// V0.1
// Reset input used to trigger start 

// L298N dual H bridge inputs
#define IN1 4
#define IN2 0
#define IN3 1
#define IN4 2

#define CAM_SW 3

void setup()
{
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  digitalWrite(IN1,LOW);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,LOW);

  pinMode(CAM_SW,INPUT_PULLUP);

  digitalWrite(IN1,HIGH);
  delay(50);
  
  while(!digitalRead(CAM_SW));
  delay(50);
  
  while(digitalRead(CAM_SW));
  delay(50);
  
  digitalWrite(IN1,LOW);

}

void loop()
{
}


