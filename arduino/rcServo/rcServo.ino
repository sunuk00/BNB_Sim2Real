// FILE: rcServo.ino
// AUTH: P. Oh
// VERS: Released 06/16/26
// DESC: 사용자가 입력한 각도(-90도 ~ +90도)로 GeekServo 5KG 서보 모터를 회전시키는 테스트 코드

// [핀 연결 가이드]
// Nano Pin 12 (D09) -> Servo PWM 신호선 (노란색/YLW)
// Nano Pin 27 (5V)  -> Servo +5V 전원선 (빨간색/RED)
// Nano Pin 29 (GND) -> Servo GND 접지선 (검은색/BLK)

#include <Wire.h>
#include <Servo.h>
#include "src/bnbPot.h"
#include "src/bnbKeyboard.h"
#include "src/bnbInit.h"

// ==========================================
// 전역 변수 선언
// ==========================================
int uTheta; // [deg] 사용자가 키보드로 입력한 목표 각도 (User desired crank angle)

// ==========================================
// 시스템 초기화 루틴
// ==========================================
void setup() {
    // Step 1: BNB 시스템 변수 및 서보/시리얼 초기화
    if (init_bnbVariables() == true) {
        Serial.println("\n\nAll variables initialized");
    }
} // end setup

// ==========================================
// 메인 제어 루프
// ==========================================
void loop() {
    pTic = millis(); // 타이머 시작 시간 기록
    
    do {
        // Step 2: 사용자로부터 원하는 각도(정수) 입력받기
        Serial.print("\nType integer angle e.g. 90 then ENTER: ");
        uTheta = key_integer(); // 정수 입력 파싱 함수 호출
        
        // Step 3: 사용자 각도(-90도 ~ +90도)를 서보 모터 동작 범위(180도 ~ 0도)로 맵핑
        mTheta = map(uTheta, -90, 90, 180, 0);
        
        // Step 4: 맵핑된 각도를 서보 모터에 전달하여 회전
        myServo.write(mTheta);
        Serial.print("Mapped crank angle in GeekServo frame: ");
        Serial.println(mTheta);
        
        // Step 5: 3초 동안 종료(q) 입력 대기
        Serial.println();
        Serial.println("\nYou have 3 sec to push q to QUIT...");
        delay(3000);
        
    } while (!key_q_pushed()); // 사용자가 'q' 키를 누르지 않았다면 각도 입력 반복
    
    // Step 6: 'q'를 눌렀을 때 프로그램 정상 종료
    Serial.println("\nBye!");
    delay(100);
    exit(0);
} // end loop
