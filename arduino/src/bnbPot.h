// FILE: bnbPot.h
// AUTH: P. Oh
// DESC: Header file for SoftPot constants and functions
// VERS: Released 06/16/26

// A3  (Nano Pin 22) -> Spectral Symbol OUT (WHITE or YLW)
// GND (Nano Pin 28) -> Spectral Symbol GND (GREY or GRN)
// 5V  (Nano Pin 27) -> Spectral Symbol VCC (PURPLE or ORG)
// D09 (Nano Pin 12) -> RC Servo S (YLW)
// GND (Nano Pin 28) -> Wall wart BLK GND and RC Servo G
// VIN (Nano Pin 30) -> Wall wart RED +9V and RC Servo V

#define VCC 5.0                  // [V] SoftPot voltage source
#define L 0.3                    // [m] SoftPot length is 0.3 meters
#define METERS_PER_VOLT 0.06     // [m/V] Conversion constant

float pot_read_position(void) {
    // Reads SoftPot OUT value and converts it into a distance
    // Returns distance in [m]
    int raw;                     // [0-1023] 10-bit ADC value
    float volts;                 // [V] acquired voltage
    float ballPosition;          // [m] position on SoftPot
    int pinPot;                  // Nano Pin Name A3 (Pin #22) <-> SoftPot OUT
    
    pinPot = A3;
    raw = analogRead(pinPot);
    volts = (raw / 1023.0) * VCC; // [V] convert to volts (0.0 to 5.0)
    ballPosition = (VCC - volts) * METERS_PER_VOLT; // [m] calculate corresponding meters
    
    return ballPosition;
} // end pot_read_position
