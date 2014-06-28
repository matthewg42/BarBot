
// Nottinghack Barbot Stirrer
// ATtiny85
// V0.1
// Reset input used to trigger start

// L298N dual H bridge inputs
// IN2 is connect to GND
#define IN1 0   // PWM
#define IN3 1   // PWM
#define IN4 2   // digital output

void setup()
{
  pinMode(IN4,OUTPUT);
  digitalWrite(IN4,LOW);

  analogWrite(IN1,0);
  analogWrite(IN3,0);

  // move down slowly
  digitalWrite(IN4,HIGH);
  analogWrite(IN3,225);
  delay(6000);

  // motor off
  digitalWrite(IN4,LOW);
  analogWrite(IN3,0);
  delay(500);

  // stir 
  analogWrite(IN1,220);
  delay(6000);
 
  // stir off
  analogWrite(IN1,0);
  delay(1000);

  // move up slowly
  digitalWrite(IN4,LOW);
  analogWrite(IN3,80);
  delay(500);

  // motor off
  digitalWrite(IN4,LOW);
  analogWrite(IN3,0);

  // stir 
  analogWrite(IN1,220);
  delay(200);

  // stir off
  analogWrite(IN1,0);
  delay(200);

  // move up fast
  digitalWrite(IN4,LOW);
  analogWrite(IN3,255);
  delay(1000);
      
  // motor off
  digitalWrite(IN4,LOW);
  analogWrite(IN3,0);
}

void loop()
{
}


