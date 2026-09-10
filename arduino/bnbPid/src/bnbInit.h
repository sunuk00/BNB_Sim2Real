// FILE: bnbInit.h
// AUTH: P. Oh
// DESC: H-file to declare and initialize BNB
// VERS: Released 06/19/26

#include <Servo.h>

// Global variables for servo and crank angle
Servo myServo;            // create servo object
int theta;                // [deg] user desired crank angle
float fTheta;             // [deg] theta, but as float
float fTheta_RAD;         // [rad] theta in radians
float fThetaPrev_RAD;     // [rad]
int mTheta;               // [deg] mapped crank angle
int pinServo;             // Nano pin number 9 (D09) for servo

// Global variables for ball state
float rD;                 // [m] desired position
float rZ;                 // [m] zero position
float r;                  // [m] position
float rPrev;              // [m] previous position
float rDot;               // [m/s] velocity
float rE;                 // [m] position tracking error
float rEPrev;             // [m] previous position tracking error
float rEDot;              // [m/s] velocity tracking error
float rESum;              // [m] sum of position errors

// Global variables for timing
long pTic, cTic, dTic;    // [ms] previous, current and delta ticks
float dTic_SEC;           // [s] delta tick difference
float eTime_SEC;          // [s] elapsed time since starting
float rTime_SEC;          // [s] desired run time to acquire data
float sTime_SEC;          // [s] sampling time
float wTime_SEC;          // [s] wait time until looping again
long wTime_MSEC;          // [ms] wait time until looping gain
float epsilonTime;        // [s] time to quit program

bool init_bnbVariables(void) {
    // DESC: Initialize all variables, baud, timing and servo
    pinServo = 9;         // Nano pin number 9 (D09) for servo
    
    // Initialize ball state to zero
    rD = rZ = r = rPrev = rDot = 0.0;
    
    // Initialize all errors to zero
    rESum = rEDot = rEPrev = 0.0;
    
    // Initialize crank angle to zero
    fTheta = theta = fTheta_RAD = fThetaPrev_RAD = 0.0;
    
    // Initialize timing variables
    sTime_SEC = 0.020;    // [sec] sampling time e.g. 0.020 sec
    rTime_SEC = 60.0;     // [sec] total run time e.g. 30.0 sec
    eTime_SEC = 0.0001;   // [sec] set elapsed time to zero
    epsilonTime = 0.0001; // [sec] reached rTime_SEC within epsilon
    
    myServo.attach(pinServo, 1000, 2000);
    Serial.begin(115200);
    delay(500);
    Serial.println("115200 baud initialized");
    
    mTheta = map(theta, -90, 90, 0, 180);
    myServo.write(mTheta);
    
    Serial.print("theta = ");
    Serial.println(theta);
    Serial.print("mTheta = ");
    Serial.println(mTheta);
    Serial.println("Servo initialized with theta = 0");
    Serial.println();
    
    Serial.println("Press c on keyboard to CONTINUE");
    do {
        delay(200);
    } while (!key_c_pushed());
    
    return true;
} // end init_bnbVariables

void init_bnbPositions(void) {
    // DESC: Define ball's desired (rD) and zero (rZ) positions
    Serial.println("\nStep 1: Center ball on beam. This will be the DESIRED position");
    Serial.println("Press d on keyboard when done");
    
    do {
        rD = pot_read_position();
        Serial.print("rD = ");
        Serial.println(rD, 4);
        delay(500); // 500 ms is good update rate to display data
    } while (!key_d_pushed());
    
    Serial.println("\nStep 2: Roll ball to origin");
    Serial.println("Press z to ZERO ball 's position");
    
    do {
        rZ = pot_read_position();
        Serial.print("rZ = ");
        Serial.println(rZ, 4);
        delay(500); // 500 ms is good update rate to display data
    } while (!key_z_pushed()); // end do-while
    
    // Prompt user to begin
    Serial.println("\nStep 3: Set CoolTerm to save data to file and/or Push g to GO");
    Serial.println("Once started, push q to QUIT");
    
    do {
        delay(100);
    } while (!key_g_pushed()); // end do-while
} // end init_bnbPositions

void init_time_delay(void) {
    // DESC: Computes time to wait before looping for next sample
    cTic = millis();                 // [ms] read timer value
    dTic = cTic - pTic;              // [ms] time between ticks
    dTic_SEC = dTic / 1000.0;        // [sec] time between ticks
    wTime_SEC = sTime_SEC - dTic_SEC;// [sec] wait time
    wTime_MSEC = wTime_SEC * 1000;   // [ms] wait time
    delay(wTime_MSEC);
    
    // Measure elapsed time
    cTic = millis();                 // [ms] read timer value
    dTic = cTic - pTic;              // [ms] time between ticks
    dTic_SEC = dTic / 1000.0;        // [sec] time between ticks
    eTime_SEC = eTime_SEC + dTic_SEC;
    
    // Update current tic value and position error
    pTic = cTic;
    rEPrev = rE;                     // used in bnbPid2_0a but unused in bnbLead2_0a
    rPrev = r;                       // added for bnbLead2_0a, bnbPP2_0a, bnbLqr2_0a, bnbFuzzy2_0a
} // end init_time_delay
