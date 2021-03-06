#include <Servo.h>

// Variable Setup 
int capValue[5];
int capSense[5];
int capThreshold;
int ledPins[5];
int testAngle;
int emojiToggle;
int emojiState;
int emojiSpeed;
int activeServos;
int servoDirection[4];
int servoAngle[4];
int armMin;
int armMax;
int analogIn;
Servo roboServo[4];

void setup() {

  // Servo Setup
  roboServo[0].attach(7); // Pin 7: fullServo
  roboServo[1].attach(3); // Pin 3: handServo
  //roboServo[2].attach(); // rotatorServo 
  //roboServo[3].attach(); // shoulderServo 
  for (int i=0;i<5;i++) {
    servoDirection[i] = 1; // (-'ve is opposite direction, can be increased if code changed below...)
  }
  activeServos = 2; // Number of servos running
  emojiSpeed = 1; // Speed in which to increment the angle of the servo

  // Emoji State (and LEDS)
  emojiToggle = 0; // 0 = unpressed, 1 = pressed
  emojiState = 0; // loops after 4...
  capThreshold = 0; 
  analogIn = 1; // Mode: analog or digital
  armMin = 20;
  armMax = 160;

  // Pin references
  if (analogIn) {
    capSense[0] = A0; 
    capSense[1] = A1; 
    capSense[2] = A2;
    capSense[3] = A3;
    capSense[4] = A4;
  } else {
    capSense[0] = 14; 
    capSense[1] = 15;
    capSense[2] = 16;
    capSense[3] = 17;
    capSense[4] = 18;
  }
  ledPins[0] = 6; //These are analog out pins (PWM)
  ledPins[1] = 9; 
  ledPins[2] = 10;
  ledPins[3] = 11; 
  ledPins[4] = 12;

  // Set up pin I/O
  for (int i=0;i<5;i++) { 
    pinMode(capSense[i], INPUT);
    pinMode(ledPins[i], OUTPUT);
  }

  // Default Serial Value (9600)
  Serial.begin(9600);
}

int startPress; // -1
int startRelease; // -1
int counterTarget; //200?

void loop() {

  // Trigger LEDs with Capacitor Touch Values
  for (int i=0;i<5;i++) { // looping through all sensor inputs
    
    if (analogIn) { // Analog or digital read
      capValue[i] = analogRead(capSense[i]); // get value from capacitor sensor
      capValue[i] = map(capValue[i], 0, 1023, 0, 255);
    } else { 
      capValue[i] = digitalRead(capSense[i]);
    }
    
    if (capValue[i] > capThreshold) { // High recieved

      int activate = 0;
      if (startPress == -1) {
        startPress = millis()
        startRelease = -1;
      } else {
          if (startPress + counterTarget == millis()) {
            activate = 1;
          }
      }
      
      // Activate Emoji Toggle
      if (i == 0 && activate) {
        
        // Should toggle-trigger a state change 
        emojiState++;

        Serial.println("Toggle Flag\n");
        
        if (emojiState >= 5) { // Loop Emoji State
          emojiState = 0;  
        }
        
      } else if (i != 0 && i < activeServos) { // Half servos 1-3

        Serial.println("Servo\n");

        // Flip Direction at Limits
        if ((servoAngle[i] <= armMin && servoDirection[i] == -1) || (servoAngle[i] >= armMax && servoDirection[i] == 1)) {
          servoDirection[i] *= -1;    
          
        }
        // Increment servo angle
        servoAngle[i] += servoDirection[i];

        // Apply angle to servo
        roboServo[i].write(servoAngle[i]);

        // STUB: reverse direction on release?
        
      }
      
    } else { // Low received
        
      startPress = -1;
    
    }

    if (i == emojiState) { // Emoji Light
      // Move to target angle for select emoji
      int penta = 72;
      if (servoAngle[0] > emojiState * penta) {
        servoAngle[0] -= emojiSpeed;
        roboServo[i].write(servoAngle[0]);
      } else if (servoAngle[0] < emojiState * penta) {
        servoAngle[0] += emojiSpeed;
        roboServo[i].write(servoAngle[0]);
      } else if (servoAngle[0] == emojiState * penta) {
        // drive LED
        if (analogIn) {
          analogWrite(ledPins[emojiState], capValue[i]); 
        } else {
          digitalWrite(ledPins[emojiState], HIGH);
        }
      }
      
    } else { // Unused LEDs
      if (analogIn) {
        analogWrite(ledPins[i], 0);  
      } else {
        digitalWrite(ledPins[i], LOW);  
      } 
    }
  }

  
}
