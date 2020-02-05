/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.

  Improved Keyboard example

  Shows how to use the new Keyboard API.

  See HID Project documentation for more information.
  https://github.com/NicoHood/HID/wiki/Keyboard-API#improved-keyboard
*/

#include "HID-Project.h"
#define ALL_STATES -1
#define BUTTON_NO   5
#define NEXT_PIN   10
#define PREV_PIN   11
#define PLAY_PIN   12
#define MUTE_PIN    2
#define MICM_PIN    9

class button {
  private:
    int triggerState;
  public:
    int pin, key, state, prevState;
    unsigned long changeTime;
    explicit button(int pin, int key, int triggerState):
      pin(pin), key(key), state(HIGH), prevState(HIGH), triggerState(triggerState), changeTime(millis()) {
      pinMode(pin, INPUT_PULLUP);
    }
    bool shouldTrigger() {
      if (triggerState == ALL_STATES) return true;
      return state == triggerState;
    }
    virtual void callback() = 0;
    String string() {
      return String("pin: " + String(pin) + ", key: " + String(key) + ", triggerState: " + String(triggerState));
    }
};

class mediaButton: public button {
    using button::button;
    void callback() {
      Consumer.write(key);
    }
};

class micMuteButton: public button {
    using button::button;
    void callback() {
      Keyboard.press(KEY_LEFT_GUI);
      Keyboard.press(KEY_F4);
      Keyboard.releaseAll();
    }
};

button *allButtons[BUTTON_NO];

const int rotOutputA = 3;
const int rotOutputB = 4;
int aLastState;

void setup() {
  Serial.begin(9600);

  allButtons[0] = new mediaButton(NEXT_PIN, MEDIA_NEXT, LOW);
  allButtons[1] = new mediaButton(PREV_PIN, MEDIA_PREV, LOW);
  allButtons[2] = new mediaButton(PLAY_PIN, MEDIA_PLAY_PAUSE, LOW);
  allButtons[3] = new mediaButton(MUTE_PIN, MEDIA_VOL_MUTE, LOW);
  allButtons[4] = new micMuteButton(MICM_PIN, MEDIA_VOL_MUTE, ALL_STATES);

  // Sends a clean report to the host. This is important on any Arduino type.
  Consumer.begin();
  Keyboard.begin();

  pinMode (rotOutputA, INPUT);
  pinMode (rotOutputB, INPUT);
  aLastState = digitalRead(rotOutputA);
}


unsigned long nowTime;
int aState;

void loop() {
  // ROTARY ENCODER
  aState = digitalRead(rotOutputA);
  if (aState != aLastState) {
    if (digitalRead(rotOutputB) == aState) {
      Consumer.write(MEDIA_VOL_UP);
    } else {
      Consumer.write(MEDIA_VOL_DOWN);
    }
  }
  aLastState = aState;

  // BUTTONS
  nowTime = millis();
  for (int i = 0; i < BUTTON_NO; i++) {
    allButtons[i]->state = digitalRead(allButtons[i]->pin);
    // this fancy debounce mechanism is needed because the rotary encoder won't work with a normal delay(100)
    if (nowTime - allButtons[i]->changeTime > 100 && allButtons[i]->prevState != allButtons[i]->state) {
      allButtons[i]->changeTime = nowTime;
      if (allButtons[i]->shouldTrigger()) {
        allButtons[i]->callback();
        Serial.println(allButtons[i]->string());
      }
    }
    allButtons[i]->prevState = allButtons[i]->state;
  }
}
