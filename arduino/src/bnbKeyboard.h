// FILE: bnbKeyboard.h
// AUTH: P. Oh
// DESC: Header file detects if keyboard key was pushed
// VERS: Released 06/16/26

bool key_d_pushed(void) {
    char receivedChar;           // Read serial terminal for char input
    bool dKey;                   // (d)esired keyboard button
    dKey = false;
    
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        if (receivedChar == 'd') {
            dKey = true;         // d key was pushed
        } else {
            dKey = false;
        }
    } // end if
    
    return dKey;
} // end key_d_pushed

bool key_q_pushed(void) {
    char receivedChar;           // Read serial terminal for char input
    bool qKey;                   // (q)uit keyboard button
    qKey = false;
    
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        if (receivedChar == 'q') {
            qKey = true;         // q key was pushed
        } else {
            qKey = false;
        }
    } // end if
    
    return qKey;
} // end key_q_pushed

bool key_z_pushed(void) {
    char receivedChar;           // Read serial terminal for char input
    bool zKey;                   // (z)ero keyboard button
    zKey = false;
    
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        if (receivedChar == 'z') {
            zKey = true;         // z key was pushed
        } else {
            zKey = false;
        }
    } // end if
    
    return zKey;
} // end key_z_pushed

bool key_g_pushed(void) {
    char receivedChar;           // Read serial terminal for char input
    bool gKey;                   // (g)o keyboard button
    gKey = false;
    
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        if (receivedChar == 'g') {
            gKey = true;         // g key was pushed
        } else {
            gKey = false;
        }
    } // end if
    
    return gKey;
} // end key_g_pushed

bool key_c_pushed(void) {
    char receivedChar;           // Read serial terminal for char input
    bool cKey;                   // (c)ontinue keyboard button
    cKey = false;
    
    if (Serial.available() > 0) {
        receivedChar = Serial.read();
        if (receivedChar == 'c') {
            cKey = true;         // c key was pushed
        } else {
            cKey = false;
        }
    } // end if
    
    return cKey;
} // end key_c_pushed

int key_integer(void) {
    // DESC: Return the integer the user typed on keyboard
    String strNumber;            // string version of number entered
    int intNumber;               // int version of number entered
    intNumber = 0;               // initialization
    
    do {
        delay(100);              // do nothing until user types digits
    } while (Serial.available() == 0);
    
    if (Serial.available() > 0) {
        Serial.setTimeout(5000); // Gives user 5 sec to enter digits
        strNumber = Serial.readStringUntil('\n'); // ENTER key pushed
        intNumber = strNumber.toInt();
        
        // clear serial buffer after reading
        while (Serial.available() > 0) {
            Serial.read();
        }
    } // end if
    
    return intNumber;
} // end key_integer

float key_float(void) {
    // DESC: Return the float the user typed on keyboard
    float fNumber;               // float version of number entered
    fNumber = 0.0;               // initialization
    
    do {
        delay(100);              // do nothing until user types digits
    } while (Serial.available() == 0);
    
    if (Serial.available() > 0) {
        Serial.setTimeout(5000); // Gives user 5 sec to enter digits
        fNumber = Serial.parseFloat();
        
        // clear serial buffer after reading
        while (Serial.available() > 0) {
            Serial.read();
        }
    } // end if
    
    return fNumber;
} // end key_float
