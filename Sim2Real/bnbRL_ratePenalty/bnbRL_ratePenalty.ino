// FILE: bnbRL.ino
// DESC: RL policy control on real BNB hardware
// VERS: 1.0 policy=exp01 (baseline)                       - works
//     : 1.1 calibration ADC_HALF 2047.5 -> 1540           - works
//     : 2.0 policy=exp07 (rate penalty w_r=0.2)           - chattering reduced

#include <ESP32Servo.h>
#include "src/bnbInfer.h"

const int POT_PIN   = 34;
const int SERVO_PIN = 18;

// --- Coordinate transform ---------------------------------------------
// Uncalibrated: assumes the ball's center sits at mid-ADC (2047.5).
//               Not physically measured, but what v1.0/v2.0 logs used.
// Calibrated  : from measured end stops, ADC_LEFT = 260.5, ADC_RIGHT = 3340.5
//               -> center 1800.5, half 1540
// Swap the two blocks below to compare.
//const float ADC_LEFT   = 0.0f;
//const float ADC_RIGHT  = 4095.0f;
const float ADC_LEFT   = 260.5f;     // calibrated
const float ADC_RIGHT  = 3340.5f;    // calibrated

const float ADC_CENTER = (ADC_LEFT + ADC_RIGHT) / 2.0f;
const float ADC_HALF   = (ADC_RIGHT - ADC_LEFT) / 2.0f;
const float X_MAX      = 0.144f;       // range the policy was trained on
// ---------------------------------------------------------------------

// --- Timing ---
const float TS = 0.020f;              // [s]
const unsigned long TS_MS = 20;

// --- Velocity filter ---
const float FILT_A = 0.7f;            // 0 = no filtering, 1 = fully smoothed

Servo myServo;
float xPrev = 0.0f, xdotPrev = 0.0f;
unsigned long pTic;

float readPosition() {
  int raw = analogRead(POT_PIN);
  return (raw - ADC_CENTER) / ADC_HALF * X_MAX;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  myServo.attach(SERVO_PIN, 1000, 2000);
  myServo.write(90);                  // neutral
  delay(1000);

  Serial.printf("# ADC_CENTER=%.1f  ADC_HALF=%.1f\n", ADC_CENTER, ADC_HALF);

  xPrev = readPosition();
  xdotPrev = 0.0f;
  pTic = millis();

  Serial.println("t,x,xdot,alpha");
}

void loop() {
  // 1. Sensor
  float x = readPosition();

  // 2. Velocity estimation
  float xdotRaw = (x - xPrev) / TS;
  float xdot = FILT_A * xdotPrev + (1.0f - FILT_A) * xdotRaw;

  // 3. Inference
  float alpha = policy_forward(x, xdot);

  // 4. Sign flip (hardware direction is opposite of simulation)
  float alphaCmd = -alpha;

  // 5. Servo
  int theta = (int)alphaCmd;
  int mTheta = map(theta, -90, 90, 180, 0);
  myServo.write(mTheta);

  // 6. Logging
  Serial.printf("%lu,%.4f,%.4f,%.2f\n", millis(), x, xdot, alphaCmd);

  // 7. Update state and maintain loop period
  xPrev = x;
  xdotPrev = xdot;
  while (millis() - pTic < TS_MS) { }
  pTic = millis();
}