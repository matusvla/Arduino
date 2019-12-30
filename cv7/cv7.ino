// Bzučák
// Bzučák můžeš využít kdykoli, pokud budeš chtít použít nějaký zvuk.
// Email:laskarduino@gmail.com
// Web:laskarduino.cz
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/
int buzzer = 12; // číslo pinu bzučáku
void setup() {
  pinMode(buzzer, OUTPUT); // nastavení pinu bzučáku jako výstupu
}
void loop() {
  unsigned char i; // definuj proměnnou
    // generace frekvence
      digitalWrite(buzzer, HIGH);
      delay(20); // počkat 1ms
      digitalWrite(buzzer, LOW);
      delay(10000); // počkat 1ms
}
