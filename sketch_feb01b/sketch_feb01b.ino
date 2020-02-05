/*     Arduino Rotary Encoder Tutorial

    by Dejan Nedelkovski, www.HowToMechatronics.com

*/

#define outputA 3
#define outputB 4
int counter = 0;
int aState;
int bState;
int aPrev = 0;
int bPrev = 0;
void setup() {
  pinMode (outputA, INPUT);
  pinMode (outputB, INPUT);

  Serial.begin (9600);
  // Reads the initial state of the outputA
}
void loop() {
  aState = digitalRead(outputA); // Reads the "current" state of the outputA
  // If the previous and the current state of the outputA are different, that means a Pulse has occured
  if (aPrev != aState) {
    if (digitalRead(outputB) != aState) {
      counter ++;
    } else {
      counter --;
    }
    aPrev = aState;
    Serial.println(counter);
  }
}
