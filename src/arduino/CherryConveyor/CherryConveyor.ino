
// Nottinghack Barbot Cherry Conveyor
// ATtiny85
// V0.1
// Reset input used to trigger start 

// L298N dual H bridge inputs
#define IN1 4
#define IN2 0
#define IN3 1
#define IN4 2

// IR sensor
#define IR_SENSOR 3
#define CHERRY    HIGH
#define NO_CHERRY LOW

// Conveyor interval between steps in milliseconds
#define CONV_DELAY 20

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

  pinMode(IR_SENSOR,INPUT_PULLUP);

  while(digitalRead(IR_SENSOR)!=CHERRY)     // whilst no cherry, keep stepping forward
    StepperFwd(1);

  while(digitalRead(IR_SENSOR)==CHERRY)     // wait for cherry to clear sensor
    StepperFwd(1);

  StepperRev(4);                            // back off a little to stop further cherries falling off
}

void loop()
{
  // reflect IR sensor state to IN1 and wait for RESET to start cycle
  digitalWrite(IN1,digitalRead(IR_SENSOR));
  
}

void StepperFwd(int steps)
{
  while(steps-->0)
  {  
    digitalWrite(IN1,HIGH);
    digitalWrite(IN4,LOW);
    delay(CONV_DELAY);
  
    digitalWrite(IN2,HIGH);
    digitalWrite(IN1,LOW);
    delay(CONV_DELAY);

    digitalWrite(IN3,HIGH);
    digitalWrite(IN2,LOW);
    delay(CONV_DELAY);

    digitalWrite(IN4,HIGH);
    digitalWrite(IN3,LOW);
    delay(CONV_DELAY);
  }

  digitalWrite(IN4,LOW);
}

void StepperRev(int steps)
{
  while(steps-->0)
  {  
    digitalWrite(IN4,HIGH);
    digitalWrite(IN1,LOW);
    delay(CONV_DELAY);
  
    digitalWrite(IN3,HIGH);
    digitalWrite(IN4,LOW);
    delay(CONV_DELAY);

    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    delay(CONV_DELAY);

    digitalWrite(IN1,HIGH);
    digitalWrite(IN2,LOW);
    delay(CONV_DELAY);
 }

  digitalWrite(IN1,LOW);
}
