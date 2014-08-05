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

#define CMD_BUF_SZ          4

#define POS_OPEN            0
#define POS_CLOSED          1
#define POS_SERVICE         2
#define POS_DISPENSE        3

#define PLATFORM_MAX_SPEED  1500
#define PLATFORM_ACCEL      3000

#define OPTIC_DELAY         3000
#define MIXER_DELAY         2000

#define ZERO_SWITCH_PIN     52
#define EMERGENCY_STOP_PIN  53
#define UMBRELLA_PIN        32
#define LEMON_PIN           34
#define STIRRER_PIN         36
#define CONVEYOR_PIN        38
#define CONVEYOR_SENSOR_PIN 39

#define LAST_DISPENSER_ID   18

int dispenserPositions[] = { 0, 546, 1093, 1640, 2187, 2734, 3117, 3417, 3746, 4057, 4375, 4714, 5058, 5332, 5605, 6042, 6485, 6792, 7000 };

Servo servoOptic[6];
uint8_t opticServoPin[]        = { 40, 42, 44, 46, 48, 50 };
uint8_t opticServoClosedPos[]  = { 10, 10, 10, 65, 10, 10 };
uint8_t opticServoOpenPos[]    = { 65, 65, 65, 10, 65, 65 };
uint8_t opticServoServicePos[] = {  0,  0,  0, 75,  0,  0 };

Servo servoMixer[6];
uint8_t mixerServoPin[]       = { 41, 43, 45, 47, 49, 51 };
uint8_t mixerServoClosedPos[] = { 140, 140, 140, 140, 140, 140 };
uint8_t mixerServoOpenPos[]   = { 65, 65, 65, 65, 65, 65 };

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
    digitalWrite(CONVEYOR_PIN,HIGH);

    pinMode(ZERO_SWITCH_PIN,INPUT_PULLUP);  // zero switch
    pinMode(EMERGENCY_STOP_PIN,INPUT_PULLUP);  // emergency stop switch

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
        id = atoi(cmdbuf+1);
        Serial.print("Go ");
        Serial.println(id);
        gotoDispenser(id);
        break;
    case 'M':
        if (cmdbuf[1] == 'F') {
            mixerFlourish();
        }
        else if (cmdbuf[1] == 'O') {
            id = atoi(cmdbuf+2);
            setMixer(id, POS_OPEN);
        }
        else if (cmdbuf[1] == 'C') {
            id = atoi(cmdbuf+2);
            setMixer(id, POS_CLOSED);
        }
        else if (cmdbuf[1] == 'D') {
            id = atoi(cmdbuf+2);
            setMixer(id, POS_DISPENSE);
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

void printHelp(void) {
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
}
