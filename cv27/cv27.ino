// Servo
// Email:laskarduino@gmail.com
// Web:laskarduino.cz
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/
#include <Servo.h>
Servo myservo; // vytvořít objekt pro řízení serva
const int servoPin = 9; // číslo pinu serva
int pos = 0; // proměnna pro ukládání pozice serva
void setup() {
  Serial.begin(9600);
  myservo.attach(servoPin); // nastavení pinu serva
}
void loop() {
  for (int i = -100; i < 300; i += 10) {
      myservo.write(i);
      delay (1000);
      Serial.println(String(i));
  }
}
