// FILE: bnbRL.ino
// DESC: RL policy control on real BNB hardware
// VERS: 1.0 baseline - works!
//     : 1.1 좌표 변환 값 변경해보기 - 

#include <ESP32Servo.h>
#include "src/bnbInfer.h"

const int POT_PIN   = 34;
const int SERVO_PIN = 18;

// --- 좌표 변환 (임시값, 캘리브레이션 후 수정) ---
const float ADC_LEFT   = 260.5f;      // 측정값
const float ADC_RIGHT  = 3340.5f;;      // 측정값
const float ADC_CENTER = (ADC_LEFT + ADC_RIGHT) / 2.0f;
const float ADC_HALF   = (ADC_RIGHT - ADC_LEFT) / 2.0f;
//const float ADC_HALF = 1800.0f;   // 1540과 2047.5 사이
const float X_MAX      = 0.114f;   // 정책이 학습한 범위

// --- 타이밍 ---
const float TS = 0.020f;              // [s]
const unsigned long TS_MS = 20;

// --- 속도 필터 ---
const float FILT_A = 0.7f;            // 0=필터없음, 1=완전평활

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
  myServo.write(90);                  // 중립
  delay(1000);

  xPrev = readPosition();
  xdotPrev = 0.0f;
  pTic = millis();

  Serial.println("t,x,xdot,alpha");
}

void loop() {
  // ① 센서
  float x = readPosition();

  // ② 속도 추정
  float xdotRaw = (x - xPrev) / TS;
  float xdot = FILT_A * xdotPrev + (1.0f - FILT_A) * xdotRaw;

  // ③ 추론
  float alpha = policy_forward(x, xdot);

  // ④ 부호 반전 (실기가 시뮬과 반대)
  float alphaCmd = -alpha;

  // ⑤ 서보
  int theta = (int)alphaCmd;
  int mTheta = map(theta, -90, 90, 180, 0);
  myServo.write(mTheta);

  // ⑥ 로깅
  Serial.printf("%lu,%.4f,%.4f,%.2f\n", millis(), x, xdot, alphaCmd);

  // ⑦ 상태 갱신 + 주기 유지
  xPrev = x;
  xdotPrev = xdot;
  while (millis() - pTic < TS_MS) { }
  pTic = millis();
}
