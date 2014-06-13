#define POWER 80

void dispense(int ms);

void setup() {
  Serial.begin(9600);
  analogWrite(3,0);
  analogWrite(5,0);
 
}

void loop() {
  Serial.println("reset...");
  analogWrite(5,255);
  delay(5000);
  analogWrite(5,0);

  Serial.println("filling...");
  analogWrite(3,255);
  delay(8000);
  analogWrite(3,0);

  delay(500);
  
  // get rid of air in tube
  dispense(1000);

  dispense(250);
  dispense(300);
  dispense(350);
  dispense(400);
  dispense(500);
  dispense(600);
  dispense(700);
  dispense(800);
  dispense(900);
  dispense(1000);

  Serial.println("WARNING!  RESET IMMINENT");  
  delay(2000);
  
}

void dispense(int ms) {
  Serial.print("dispensing for ");
  Serial.println(ms);
  analogWrite(5,POWER);
  delay(ms);  
  analogWrite(5,0);
  delay(1500);

}
