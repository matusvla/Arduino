int ledNum = 6; // počet LEDek
byte ledPin[] = { 2, 3, 4, 5, 6, 7}; // vytvoříme pole pro LED piny
int ledDelay; // pauza mezi změnami
int direction = 1;
int currentLED = 0;
unsigned long changeTime;
int potPin = A0;

void setup() {
  Serial.begin(9600);
  for (int x = 0; x < ledNum; x++) {
    // nastavení pinu od 2 do 9 LEDky jako výstupu
    pinMode(ledPin[x], OUTPUT);
  }
  changeTime = millis();
}
//
void loop() {
  int activeLed = analogRead(potPin) * (ledNum-1) / maxPotVal;
  Serial.println(activeLed);
  for (int x = 0; x < ledNum; x++) {
    // vypnout všechny LEDky
    digitalWrite(ledPin[x], LOW);
  }
  digitalWrite(ledPin[activeLed], HIGH);
}

void loop() {
  int ledDelay = analogRead(potPin); // přečíst hodnotu pinu potenciometru
  // tady se změní interval po nastavení potenciometru
  Serial.println(ledDelay);
  if ((millis() - changeTime) > ledDelay) {
    changeLED();
    changeTime = millis();
  }
}

void changeLED() {
  for (int x = 0; x < ledNum; x++) {
    // vypnout všechny LEDky
    digitalWrite(ledPin[x], LOW);
  }
  // zapnout tuto LEDku
  digitalWrite(ledPin[currentLED], HIGH);
  currentLED += direction;
  // změnit směr, pokud jsme dosáhli konce
  if (currentLED == ledNum - 1) {
    direction = -1;
  }
  if (currentLED == 0) {
    direction = 1;
  }
}
