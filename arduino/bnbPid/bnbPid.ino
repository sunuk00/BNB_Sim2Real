// FILE: bnbPid.ino
// AUTH: P. Oh
// DESC: Ball and Beam (BNB) PID 제어 실험 코드
// VERS: Released 06/19/26

#include <Wire.h>
#include <Servo.h>
#include "src/bnbPot.h"      // SoftPot 관련 위치 측정 함수
#include "src/bnbKeyboard.h" // 키보드 입력 감지 함수
#include "src/bnbInit.h"     // BNB 변수 선언 및 초기화 함수

// ==========================================
// 전역 변수 선언
// ==========================================
float kP, kI, kD; // PID 제어 게인 (Proportional, Integral, Derivative)

// ==========================================
// 시스템 초기화 루틴
// ==========================================
void setup() {
    // Step 1: BNB 시스템 변수, 타이밍, 서보 및 시리얼 초기화
    if (init_bnbVariables() == true) {
        Serial.println("\n\nAll variables initialized");
    }
    
    // 사용자로부터 실시간 PID 게인 값(kP, kI, kD) 입력받기
    Serial.println("\nType kP gain (e.g. 600.0) then ENTER: ");
    kP = key_float();
    
    Serial.println("\nType kI gain (e.g. 0.01) then ENTER: ");
    kI = key_float();
    
    Serial.println("\nType kD gain (e.g. 100.0) then ENTER: ");
    kD = key_float();
    
    Serial.println("\nPID gains are: ");
    Serial.print("kP = "); Serial.println(kP);
    Serial.print("kI = "); Serial.println(kI);
    Serial.print("kD = "); Serial.println(kD);
    Serial.println();
    
    // Step 2: 공의 목표 위치(rD) 및 영점(rZ) 설정 ('d', 'z', 'g' 키 순차 진행)
    init_bnbPositions();
    
    // Step 3: 데이터 로깅용 CSV 헤더 출력
    Serial.println("T (s), r (m), rE (m), rEDot (m/s), theta (deg)");
} // end setup

// ==========================================
// 실시간 PID 제어 메인 루프
// ==========================================
void loop() {
    pTic = millis(); // 타이머 시작 시간 기록
    
    do {
        // Step 4: SoftPot 센서로부터 공의 현재 위치 r(m) 읽기
        r = pot_read_position();
        
        // Step 5: 제어 법칙(Control Law) 계산
        rE = rD - r;                                // [m] 위치 오차 (목표 위치 - 현재 위치)
        rEDot = (rE - rEPrev) / sTime_SEC;          // [m/s] 오차의 수치 미분값 (오차 변화율)
        rESum = rESum + (rE * sTime_SEC);           // [m*s] 오차의 수치 적분값 (누적 오차)
        
        // PID 제어 출력 계산 (크랭크 각도 산출)
        fTheta = (kP * rE) + (kI * rESum) + (kD * rEDot);
        
        // 서보 모터 및 기계적 한계를 고려한 각도 리미터 (-89도 ~ +89도)
        if (fTheta < -89.0) fTheta = -89.0;
        if (fTheta > +89.0) fTheta = +89.0;
        
        // Step 6: 서보 모터 구동 명령
        theta = (int)(fTheta);                      // 정수형 각도로 변환
        mTheta = map(theta, -90, 90, 180, 0);       // 서보 펄스 범위(180도 ~ 0도)로 매핑
        myServo.write(mTheta);                      // 서보 모터 회전
        
        // Step 7: 실시간 상태 데이터를 CSV 포맷으로 출력
        Serial.print(eTime_SEC); Serial.print(",");
        Serial.print(r, 4);      Serial.print(",");
        Serial.print(rE, 4);     Serial.print(",");
        Serial.print(rEDot, 4);  Serial.print(",");
        Serial.print(theta);
        Serial.println();
        
        // Step 8: 목표 샘플링 주기(20ms) 유지 및 이전 상태값 갱신
        init_time_delay();
        
    } while (
        // 실험 시간(30초) 이내이면서 'q' 키를 누르지 않았을 때 반복
        ((rTime_SEC - eTime_SEC) > epsilonTime)
        && (!key_q_pushed())
    ); // end do-while
    
    // Step 9: 'q'를 눌렀거나 시간이 종료되었을 때 프로그램 안전 종료
    delay(100);
    Serial.end();
    exit(0);
} // end loop
