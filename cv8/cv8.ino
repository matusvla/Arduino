// Otřesové čidlo
// Zapíná a vypíná LEDku když nakloniš čidlo.
// Email:laskarduino@gmail.com
// Web:laskarduino.cz
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/
const int ledPin = 13; // číslo pinu LEDky
const int tiltPin = 2; // číslo pinu otřesového čidla
void setup() {
  pinMode(ledPin, OUTPUT); // nastavení pinu LEDky jako výstupu
  pinMode(tiltPin, INPUT); // nastavení pinu otřesového čidla jako vstupu
  Serial.begin(115200);
}
void loop() {
  int digitalVal = digitalRead(tiltPin);// přečíst
  Serial.print(digitalVal);
  delay(300); // počkat 300ms
  if (HIGH == digitalVal) { // pokud není otřesové čidlo nakloněno
    digitalWrite(ledPin, LOW); // vypnout LEDku
    
  } else { // pokud je otřesové čidlo nakloněno
    digitalWrite(ledPin, HIGH); // zapnout LEDku
  }
}
