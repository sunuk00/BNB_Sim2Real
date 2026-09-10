// FILE: lPot.ino
// AUTH: P. Oh
// DESC: Display ball position with linear potentiometer
// VERS: Released 06/19/26

#include <Wire.h>
#include <Servo.h>
#include "src/bnbPot.h"
#include "src/bnbKeyboard.h"
#include "src/bnbInit.h"

void setup() {
    // Step 1: Initialize BNB variables
    if (init_bnbVariables() == true) {
        Serial.println("\n\nAll variables initialized");
    }
    
    // Step 2: Define rD and rZ ball positions
    init_bnbPositions();
    
    // Step 3: Print data in these columns
    Serial.println("T (s), r (m), rDot (m/s)");
} // end setup

void loop() {
    pTic = millis(); // Begin timer
    
    do {
        // Step 4: Read ball position
        r = pot_read_position();
        
        // Step 5: Control law computations
        rDot = (r - rPrev) / sTime_SEC; // ball's velocity
        
        // Step 6: Command servo motor
        // Add code involving myServo.write(mTheta) here
        
        // Step 7: Display ball's state
        Serial.print(eTime_SEC); Serial.print(",");
        Serial.print(r, 4); Serial.print(",");
        Serial.print(rDot, 4); Serial.print(",");
        Serial.println();
        
        // Step 8: Maintain desired sampling time
        init_time_delay();
        
    } while (
        ((rTime_SEC - eTime_SEC) > epsilonTime)
        && (!key_q_pushed())
    ); // end do-while
    
    // Step 9: q button pushed so QUIT
    delay(100);
    Serial.end();
    exit(0);
} // end loop
