#include <Servo.h>

#define SERVO_A 7 // Note: no semicolons in macro definitions
#define SERVO_B 8 

// Variable Setup 
int capValue[5];
int capSense[5];
int ledPins[5];

void setup() {

  // Servo Setup
  Servo armServo;
  Servo headServo;
  armServo.attach(6); // Pin 6
  headServo.attach(3); // Pin 3
  int servoTarget = 0;
  /* servoTarget Values (for armServo):
   * 0 = Shoulder Rotator
   * 1 = Shoulder Raise
   * 2 = Elbow
   * etc: undefined
   * 
   */

  // Pin references
  capSense[0] = 8;
  capSense[1] = 9;
  capSense[2] = 10;
  capSense[3] = 11;
  capSense[4] = 12;
  ledPins[0] = 5;
  ledPins[1] = 9;
  ledPins[2] = 10;
  ledPins[3] = 11;
  ledPins[4] = 13;

  // Set up pin I/O
  for (int i=0;i<5;i++) { 
    pinMode(capSense[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }

  // Default Serial Value (9600)
  Serial.begin(9600);
}

void loop() {

  // Trigger LEDs with Capacitor Touch Values
  for (int i=0;i<5;i++) { // looping through all sensor inputs
    
    capValue[i] = digitalRead(capSense[i]); // get value from capacitor sensor
    
    if (capValue[i] != 0) { // High recieved
      digitalWrite(ledPins[i], LOW); 
      Serial.print("on"); 
      
    } else { // Low received
      digitalWrite(ledPins[i], HIGH); 
    }
  }
}
