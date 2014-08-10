#include <AccelStepper.h>
#include <Servo.h>

// Function prototypes
void resetCmdBuf(void);
void handleCmd(void);
void gotoDispenser(uint8_t id);
void setOptic(uint8_t id, uint8_t pos);
void setMixer(uint8_t id, uint8_t pos);
void mixerFlourish(void);
void opticFlourish(void);
void printHelp(void);
void dasherDispense(uint8_t id);
void dasherFlourish(void);
void dispenseSyringe(int units);
void suckSyringe(int units);

#define CMD_BUF_SZ          4

#define POS_OPEN            0
#define POS_CLOSED          1
#define POS_SERVICE         2
#define POS_DISPENSE        3

#define PLATFORM_MAX_SPEED  1500
#define PLATFORM_ACCEL      3000

#define OPTIC_DELAY         3000
#define MIXER_DELAY         1800

#define ZERO_SWITCH_PIN     52
#define EMERGENCY_STOP_PIN  53
#define UMBRELLA_PIN        32
#define LEMON_PIN           34
#define STIRRER_PIN         36
#define CONVEYOR_PIN        38
#define CONVEYOR_SENSOR_PIN 39
#define SYRINGE_SUCK_PIN    5
#define SYRINGE_SQUIRT_PIN  6

#define ZERO_PIN            52
#define STOP_PIN            53

#define LAST_DISPENSER_ID   18

int dispenserPositions[] = { 0, 546, 1093, 1640, 2187, 2734, 3117, 3417, 3746, 4057, 4375, 4714, 5058, 5332, 5605, 5900, 6335, 6792, 7000 };

Servo servoOptic[6];
uint8_t opticServoPin[]        = { 40, 42, 44, 46, 48, 50 };
uint8_t opticServoClosedPos[]  = { 10, 65, 10, 65, 10, 10 };
uint8_t opticServoOpenPos[]    = { 65, 10, 65, 10, 65, 65 };
uint8_t opticServoServicePos[] = {  0,  0,  0, 75,  0,  0 };

Servo servoMixer[6];
uint8_t mixerServoPin[]       = { 41, 43, 45, 47, 49, 51 };
uint8_t mixerServoClosedPos[] = { 140, 140, 140, 140, 140, 140 };
uint8_t mixerServoOpenPos[]   = { 65, 65, 65, 65, 65, 65 };

uint8_t dasherPinIn[]  = { 22, 24, 26 };
uint8_t dasherPinOut[] = { 23, 25, 27 };

AccelStepper stepper(AccelStepper::DRIVER);

uint8_t bufpos;
char cmdbuf[CMD_BUF_SZ+1];

void setup()
{

    stepper.setMaxSpeed(PLATFORM_MAX_SPEED);
    stepper.setAcceleration(PLATFORM_ACCEL);
    stepper.setPinsInverted(false,false,false,false,true);
    stepper.setEnablePin(4);
    stepper.disableOutputs();

    pinMode(13,OUTPUT);

    // Init optic servos
    for(uint8_t i=0; i<6; i++) {
        servoOptic[i].attach(opticServoPin[i]);  // set the pin
        setOptic(i, POS_CLOSED);                 // make sure it's closed
    }

    // Init mixer servos
    for(uint8_t i=0; i<6; i++) {
        servoMixer[i].attach(mixerServoPin[i]);  // set the pin
        setMixer(i, POS_CLOSED);                 // make sure it's closed
    }

    pinMode(UMBRELLA_PIN,OUTPUT);
    digitalWrite(UMBRELLA_PIN,LOW);

    pinMode(LEMON_PIN,OUTPUT);
    digitalWrite(LEMON_PIN,HIGH);

    pinMode(STIRRER_PIN,OUTPUT);
    digitalWrite(STIRRER_PIN,HIGH);

    pinMode(CONVEYOR_PIN,OUTPUT); // Conveyor
    pinMode(CONVEYOR_SENSOR_PIN,INPUT_PULLUP);
    digitalWrite(CONVEYOR_PIN,LOW);

    pinMode(ZERO_SWITCH_PIN,INPUT_PULLUP);  // zero switch
    pinMode(EMERGENCY_STOP_PIN,INPUT_PULLUP);  // emergency stop switch

    // Set up dashers
    for (uint8_t i=0; i<3; i++) {
        pinMode(dasherPinIn[i],INPUT_PULLUP);
        pinMode(dasherPinOut[i],OUTPUT);
        digitalWrite(dasherPinOut[i],LOW);
    }

    Serial.begin(9600);
    delay(100);
    printHelp();
    resetCmdBuf();
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

    // convert to upper case
    if (c >= 'a' && c <= 'z') {
        c -= 32;
    }

    if (c == '\n' || c == '\r') {
        handleCmd();
    }
    else if (bufpos < CMD_BUF_SZ) {
            cmdbuf[bufpos] = c;
            bufpos++;
            Serial.print(c);
    }
    else {
        Serial.println("ERR");
    }

    stepper.enableOutputs();
    digitalWrite(13,HIGH);
}

void resetCmdBuf(void) {
    memset((void*)cmdbuf, 0, sizeof(char) * (CMD_BUF_SZ+1));
    bufpos = 0;
    Serial.print("\n> ");
}

void handleCmd(void) {
    int id = -1;
    bool bad = false;
    Serial.print("\nCMD=");
    Serial.println(cmdbuf);
    switch (cmdbuf[0]) {
    case 'H':
    case '?':
        printHelp();
        break;
    case 'G':
        // Goto (id)
        if (cmdbuf[1] == 'C') id = 15;
        else if (cmdbuf[1] == 'L') id = 16;
        else if (cmdbuf[1] == 'S') id = 17;
        else if (cmdbuf[1] == 'U') id = 18;
        else id = atoi(cmdbuf+1);
        Serial.print("Go ");
        Serial.println(id);
        gotoDispenser(id);
        break;
    case 'R':
        if (cmdbuf[1] == 'D') {
            dispenseSyringe(atoi(cmdbuf+2));
        }
        else if ( cmdbuf[1] == 'S') {
            suckSyringe(atoi(cmdbuf+2));
        }
        break;
    case 'D':
        if (cmdbuf[1] == 'F') {
            dasherFlourish();
        }
        else if (cmdbuf[1] == 'D') {
            id = atoi(cmdbuf+2);
            dasherDispense(id);
        }
        else { 
            bad = true; 
        }
        break;
     case 'M':
        if (cmdbuf[1] == 'F') {
            dasherFlourish();
        }
        else if (cmdbuf[1] == 'D') {
            id = atoi(cmdbuf+2);
            dasherDispense(id);
        }
        else { 
            bad = true; 
        }
        break;
    case 'O':
        if (cmdbuf[1] == 'F') {
            opticFlourish();
        }
        else if ( cmdbuf[1] == 'S') {
            for (uint8_t id=0; id<6; id++) {
                setOptic(id, POS_SERVICE);
                delay(100);
            }
        }
        else if ( cmdbuf[1] == 'R') {
            for (uint8_t id=0; id<6; id++) {
                setOptic(id, POS_CLOSED);
                delay(100);
            }
        }
        else if (cmdbuf[1] == 'O') {
            id = atoi(cmdbuf+2);
            setOptic(id, POS_OPEN);
        }
        else if (cmdbuf[1] == 'C') {
            id = atoi(cmdbuf+2);
            setOptic(id, POS_CLOSED);
        }
        else if (cmdbuf[1] == 'D') {
            id = atoi(cmdbuf+2);
            setOptic(id, POS_DISPENSE);
        }
        else { 
            bad = true; 
        }
        break;
    case 'C':
        if (cmdbuf[1] == 'D') {
            digitalWrite(CONVEYOR_PIN, LOW);
            delay(10);
            digitalWrite(CONVEYOR_PIN, HIGH);
        }
        else if (cmdbuf[1] == 'S') {
            Serial.println(digitalRead(CONVEYOR_SENSOR_PIN));
        }
        else if (cmdbuf[1] == 'R') {
            digitalWrite(CONVEYOR_PIN, LOW);
        }
        break;
    case 'L':
        digitalWrite(LEMON_PIN, LOW);
        delay(10);
        digitalWrite(LEMON_PIN, HIGH);
        break;
    case 'S':
        digitalWrite(STIRRER_PIN, LOW);
        delay(10);
        digitalWrite(STIRRER_PIN, HIGH);
        break;
    case 'U':
        digitalWrite(UMBRELLA_PIN, HIGH);
        delay(1000);
        digitalWrite(UMBRELLA_PIN, LOW);
        break;
    default:
        bad = true;
        break;
    }

    if (bad) {
        Serial.println("ERR");
    }

    resetCmdBuf();
}

void setOptic(uint8_t id, uint8_t pos) {
    if (pos == POS_OPEN) {
        servoOptic[id].write(opticServoOpenPos[id]);
    }
    else if (pos == POS_CLOSED) {
        servoOptic[id].write(opticServoClosedPos[id]);
    }
    else if (pos == POS_SERVICE) {
        servoOptic[id].write(opticServoServicePos[id]);
    }
    else if (pos == POS_DISPENSE) {
        setOptic(id, POS_OPEN);
        delay(OPTIC_DELAY);
        setOptic(id, POS_CLOSED);
    }
}

void setMixer(uint8_t id, uint8_t pos) {
    if (pos == POS_OPEN) {
        servoMixer[id].write(mixerServoOpenPos[id]);
    }
    else if (pos == POS_CLOSED) {
        servoMixer[id].write(mixerServoClosedPos[id]);
    }
    else if (pos == POS_DISPENSE) {
        setMixer(id, POS_OPEN);
        delay(MIXER_DELAY);
        setMixer(id, POS_CLOSED);
    }
}

void gotoDispenser(uint8_t id)
{
    if (id <= LAST_DISPENSER_ID) {
        stepper.moveTo(dispenserPositions[id]);
    }
}

void mixerFlourish(void) {
    for (uint8_t id=0; id<6; id++) {
        setMixer(id, POS_OPEN);
        delay(100);
    }
    for (uint8_t id=0; id<6; id++) {
        setMixer(id, POS_CLOSED);
        delay(100);
    }
}

void opticFlourish(void) {
    for (uint8_t id=0; id<6; id++) {
        setOptic(id, POS_OPEN);
        delay(100);
    }
    for (uint8_t id=0; id<6; id++) {
        setOptic(id, POS_CLOSED);
        delay(100);
    }
}

void dasherDispense(uint8_t id)
{
    digitalWrite(dasherPinOut[id], HIGH);
    while(!digitalRead(dasherPinIn[id]));
    while(digitalRead(dasherPinIn[id]));
    while(!digitalRead(dasherPinIn[id]));
    while(digitalRead(dasherPinIn[id]));
    digitalWrite(dasherPinOut[id], LOW);
}

void dasherFlourish(void)
{
    for (uint8_t id=0; id<3; id++) {
        dasherDispense(id);
        delay(150);
    }
}

void dispenseSyringe(int units)
{
    Serial.println("dispenseSyringe");
    analogWrite(SYRINGE_SUCK_PIN,0);
    analogWrite(SYRINGE_SQUIRT_PIN,150);
    delay(200);
    analogWrite(SYRINGE_SUCK_PIN,0);
    analogWrite(SYRINGE_SQUIRT_PIN,0);
    
    analogWrite(SYRINGE_SUCK_PIN,150);
    analogWrite(SYRINGE_SQUIRT_PIN,0);
    delay(100);
    analogWrite(SYRINGE_SUCK_PIN,0);
    analogWrite(SYRINGE_SQUIRT_PIN,0);
}

void suckSyringe(int units)
{
    analogWrite(SYRINGE_SUCK_PIN,150);
    analogWrite(SYRINGE_SQUIRT_PIN,0);
    delay(100*units);
    analogWrite(SYRINGE_SUCK_PIN,0);
    analogWrite(SYRINGE_SQUIRT_PIN,0);
}

void printHelp(void) {
    Serial.println("0000000000111111111");
    Serial.println("0123456789012345678");
    Serial.println("OOOOOOMMMMMMDDDCLSU");
    Serial.println("012345012345012CLSU");
    Serial.println("Gn  : goto dispenser n");
    Serial.println("ODn : dispense optic n");
    Serial.println("OOn : optic open n");
    Serial.println("OCn : optic open n");
    Serial.println("OS  : opt service");
    Serial.println("OF  : optic flourish (warning opens all!)");
    Serial.println("MDn : dispense mix n");
    Serial.println("MOn : open mix n");
    Serial.println("MCn : close mix n");
    Serial.println("MF  : mixer flourish (warning opens all!)");
    Serial.println("DDn : dispense dasher n");
    Serial.println("DF  : dasher flourish (warning opens all!)");
    Serial.println("CD  : conveyor dispense");
    Serial.println("CR  : conveyor reset (stop)");
    Serial.println("CS  : conveyor sensor");
    Serial.println("L   : lemon");
    Serial.println("S   : stirrer");
    Serial.println("U   : umbrella");
    Serial.println("?   : help");
}

