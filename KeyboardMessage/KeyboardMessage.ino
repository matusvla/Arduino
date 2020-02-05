#include <HID-Project.h>
#include <HID-Settings.h>

// use this option for Windows and Linux:
//  char ctrlKey = KEY_LEFT_CTRL;

void setup() {
  pinMode(12, INPUT_PULLUP);
  // initialize control over the keyboard:
//  Keyboard.begin();
}

void loop() {
  while (digitalRead(12) == HIGH) {
  }
//  Keyboard.print('p');
  while (digitalRead(12) == LOW) {
  }
  delay(100);
}
