// I2C LCD displej
// Teď by jsi měl na svém displeji vidět běžící text:“Laskarduino” a "Zdravim bastlire!".
// Email:laskarduino@gmail.com
// Web:laskarduino.cz
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/
// připoj knihovny
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

int ledNum = 6; // počet LEDek
byte ledPin[] = { 2, 3, 4, 5, 6, 7}; // vytvoříme pole pro LED piny
int ledDelay; // pauza mezi změnami
int direction = 1;
int currentLED = 0;
unsigned long changeTime;
int potPin = A0;
const int maxPotVal = 1023;
int prevPotVal = -1;
int prevActiveLed = -1;
int buzzer = 12;

// inicializace knihovny displeje
LiquidCrystal_I2C lcd(0x27, 16, 2); // nastavit adresu displeje na 0x27 pro 1602 displej
void setup() {
  lcd.init();
  // inicializace lcd
  lcd.backlight(); // zapnout podsvícení

  for (int x = 0; x < ledNum; x++) {
    // nastavení pinu od 2 do 9 LEDky jako výstupu
    pinMode(ledPin[x], OUTPUT);
  }
  changeTime = millis();
  pinMode(buzzer, OUTPUT);
}

void loop() {
  int potVal = analogRead(potPin);
  if (!isClose( potVal, prevPotVal, 2)) {
    lcd.clear();
    lcd.println(potVal);
    prevPotVal = potVal;
  }
  int activeLed = potVal * (ledNum - 1) / maxPotVal;
  if (prevActiveLed != activeLed) {
    for (int x = 0; x < ledNum; x++) {
      // vypnout všechny LEDky
      digitalWrite(ledPin[x], LOW);
      digitalWrite(ledPin[activeLed], HIGH);
      prevActiveLed = activeLed;
      digitalWrite(buzzer, HIGH);
      delay(20); // počkat 1ms
      digitalWrite(buzzer, LOW);
    }
  }
}

bool isClose(int a, int b, int epsilon) {
  if (abs(a - b) < epsilon) {
    return true;
  }
  return false;
}
