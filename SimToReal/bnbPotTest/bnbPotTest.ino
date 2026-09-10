// FILE: bnbPotTest.ino
// AUTH: Taeuk Sun
// DATE: 09/09/26 09:50
// DESC: SoftPot ADC reading test

const int POT_PIN = 34;

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("SoftPot test");
}

void loop() {
  Serial.println(analogRead(POT_PIN));
  delay(100);
}
