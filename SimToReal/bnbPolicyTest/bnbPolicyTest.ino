// FILE: bnbPolicyTest.ino
// DESC: Verify ESP32 inference matches MATLAB

#include "src/bnbInfer.h"

void setup() {
  Serial.begin(115200);
  delay(1500); // 부팅 후 USB 안정화 대기
  Serial.println("\n=== SYSTEM BOOT OK ===");
}

void loop() {
  float testX[6]    = { 0.050f, -0.080f,  0.000f,  0.100f, 0.000f,  0.114f};
  float testXdot[6] = { 0.020f,  0.000f,  0.100f, -0.050f, 0.000f,  0.300f};

  Serial.println("\n--- INFERENCE TEST ---");
  Serial.println("   x        xdot       alpha");
  for (int i = 0; i < 6; i++) {
    float a = policy_forward(testX[i], testXdot[i]);
    Serial.printf("%7.3f  %7.3f  -> %9.4f\n", testX[i], testXdot[i], a);
  }

  // --- 추론 시간(Timing Benchmark) 측정 ---
  const int NUM_RUNS = 100;
  volatile float dummy = 0.0f; // 컴파일러의 루프 최적화(생략) 방지

  unsigned long t0 = micros();
  for (int k = 0; k < NUM_RUNS; k++) {
    dummy = policy_forward(0.050f, 0.020f);
  }
  unsigned long t1 = micros();

  float avgTimeUs = (float)(t1 - t0) / NUM_RUNS;
  Serial.printf("\nInference time (avg of %d runs): %.2f us (%.4f ms)\n", NUM_RUNS, avgTimeUs, avgTimeUs / 1000.0f);
  Serial.println("----------------------------------------------");

  delay(2000); // 2초마다 갱신
}
