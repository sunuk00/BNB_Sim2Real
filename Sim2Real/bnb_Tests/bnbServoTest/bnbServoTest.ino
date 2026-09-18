// FILE: bnbServoTest.ino
// DESC: Servo angle test — no ball, no policy

#include <ESP32Servo.h>

Servo myServo;
const int SERVO_PIN = 18;

void setup() {
  Serial.begin(115200);
  delay(1000);
  myServo.attach(SERVO_PIN, 1000, 2000);
  myServo.write(90);              // neutral
  Serial.println("Servo test. Type angle (-90 to 90) then ENTER");
}

void loop() {
  if (Serial.available() > 0) {
    int theta = Serial.parseInt();
    
    // Consume any trailing newline characters ('\r', '\n') and spaces
    while (Serial.available() > 0) {
      char c = Serial.peek();
      if (c == '\n' || c == '\r' || c == ' ') {
        Serial.read();
      } else {
        break;
      }
    }
    
    if (theta >= -90 && theta <= 90) {
      int mTheta = map(theta, -90, 90, 180, 0);
      myServo.write(mTheta);
      Serial.print("theta = "); Serial.print(theta);
      Serial.print("  ->  servo = "); Serial.println(mTheta);
    }
  }
  delay(50);
}
