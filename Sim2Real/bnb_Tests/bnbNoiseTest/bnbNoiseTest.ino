// FILE: bnbNoiseTest.ino
// DESC: Measure sensor noise with ball at rest

#include <ESP32Servo.h>

const int POT_PIN   = 34;
const int SERVO_PIN = 18;
const float ADC_CENTER = 2047.5f;
const float ADC_HALF   = 2047.5f;
const float X_MAX      = 0.114f;
const float TS = 0.020f;
const unsigned long TS_MS = 20;

Servo myServo;
float xPrev = 0.0f;
unsigned long pTic;

float readPosition() {
  int raw = analogRead(POT_PIN);
  return (raw - ADC_CENTER) / ADC_HALF * X_MAX;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  myServo.attach(SERVO_PIN, 1000, 2000);
  myServo.write(90);          // 수평 고정, 이후 건드리지 않음
  delay(2000);

  xPrev = readPosition();
  pTic = millis();
  Serial.println("t,raw,x,xdotRaw");
}

void loop() {
  int raw = analogRead(POT_PIN);
  float x = (raw - ADC_CENTER) / ADC_HALF * X_MAX;
  float xdotRaw = (x - xPrev) / TS;      // 필터 없는 원값

  Serial.printf("%lu,%d,%.5f,%.5f\n", millis(), raw, x, xdotRaw);

  xPrev = x;
  while (millis() - pTic < TS_MS) { }
  pTic = millis();
}
