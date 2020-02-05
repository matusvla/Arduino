#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS1302RTC.h>

#define RELAY_ON 0
#define RELAY_OFF 1
#define BUTTON_DOWN 0
#define BUTTON_UP 1

#define DS1302_GND_PIN 33
#define DS1302_VCC_PIN 35

const int upRelayPin = 4;
const int downRelayPin =  5;
const int leftButtonPin = 2;
const int rightButtonPin = 3;

LiquidCrystal_I2C lcd(0x27, 16, 2);
DS1302RTC RTC (27, 29, 31);

void setupLcdLine(int n, String contents) {
  lcd.setCursor(0, n);
  lcd.print("                           ");
  lcd.setCursor(0, n);
  lcd.print(contents);
}

String formatTime(time_t n) {
  int m = minute(n);
  int h = hour(n);
  String str;
  if (h < 10) {
    str += '0';
  }
  str  += String(h);
  str += ":";
  if (m < 10) {
    str += '0';
  }
  str += String(m);
  return str;
}

int checkButton(bool * l, bool * r) {
  int pressLength = 0;
  *l = digitalRead(leftButtonPin) == BUTTON_DOWN;
  *r =  digitalRead(rightButtonPin) == BUTTON_DOWN;
  if (*l) {
    time_t tStart = millis();
    while (digitalRead(leftButtonPin) == BUTTON_DOWN) {
      *r =  *r || digitalRead(rightButtonPin) == BUTTON_DOWN;
    }
    pressLength = millis() - tStart;
  }
  if (*r) {
    time_t tStart = millis();
    while (digitalRead(rightButtonPin) == BUTTON_DOWN) {
      *l = *l || digitalRead(leftButtonPin) == BUTTON_DOWN;
    }
    pressLength = millis() - tStart;
  }
  while (digitalRead(rightButtonPin) == BUTTON_DOWN || digitalRead(leftButtonPin) == BUTTON_DOWN) {};
  delay(30);
  return pressLength;
}

time_t changeTimeMins(time_t timeToChange, int by_minutes) {
  int by_value = by_minutes * 60;
  Serial.println("changeTimeMins: timeToChange = " + String(timeToChange) + ", by_value = " + String(by_value));
  if (by_value < 0 && timeToChange < abs(by_value)) {
    Serial.println("changeTimeMins: bottom overflow handler");
    timeToChange = 86400 - (abs(by_value) - timeToChange);
  } else {
    timeToChange += by_value;
  }
  if (timeToChange >= 86400) {
    timeToChange = timeToChange - 86400;
  }
  Serial.println("changeTimeMins: timeToChange = " + String(timeToChange));
  return timeToChange;
}

void setupTime(time_t * timeToSet) {
  setupLcdLine(1, formatTime(*timeToSet));
  while (1) {
    int lbState = digitalRead(leftButtonPin);
    if (lbState == BUTTON_DOWN) {
      delay(100);
    }
    int rbState = digitalRead(rightButtonPin);
    if (rbState == BUTTON_DOWN) {
      delay(100);
      lbState = digitalRead(leftButtonPin);
    }
    if (lbState == BUTTON_DOWN && rbState == BUTTON_DOWN) {
      while (lbState == BUTTON_DOWN || rbState == BUTTON_DOWN) {
        lbState = digitalRead(leftButtonPin);
        rbState = digitalRead(rightButtonPin);
      }
      return;
    }
    while (lbState == BUTTON_DOWN) {
      *timeToSet = changeTimeMins(*timeToSet, -1);
      setupLcdLine(1, formatTime(*timeToSet));
      //      delay(100);
      lbState = digitalRead(leftButtonPin);
    }
    while (rbState == BUTTON_DOWN) {
      *timeToSet = changeTimeMins(*timeToSet, 1);
      setupLcdLine(1, formatTime(*timeToSet));
      //      delay(100);
      rbState = digitalRead(rightButtonPin);
    }
  }


  return true;
}

//---------------------------------------------------

class Settings
{
  private:
    time_t upTime, downTime;
    int moveInterval;
    void setDownTime() {
      setupTime(&downTime);
      Serial.println("downTime = " + String(formatTime(downTime)) + " " + String(downTime));
      setupLcdLine(1, formatTime(downTime) + " " + formatTime(upTime));
    }
    void setUpTime() {
      setupTime(&upTime);
      Serial.println("downTime = " + String(formatTime(downTime)) + " " + String(downTime));
      setupLcdLine(1, formatTime(downTime) + " " + formatTime(upTime));
    }

  public:
    Settings(): moveInterval(20000), upTime(32400), downTime(75600) {};
    void configure() {
      lcd.on();
      lcd.backlight();
      setupLcdLine(0, "DOWN    UP");
      setupLcdLine(1, formatTime(downTime) + " " + formatTime(upTime));
      bool l, r = false;
      int pressLength = 0;
      while (1) {
        pressLength = checkButton(&l, &r);
        if (l & r) {
          break;
        } else if (l) {
          if (pressLength > 0 && pressLength < 1000) {
            Serial.println("L SHORT PRESS");
          } else {
            Serial.println("L LONG PRESS");
            setDownTime();
          }
        } else if (r) {
          if (pressLength > 0 && pressLength < 1000) {
            Serial.println("R SHORT PRESS");
          } else {
            Serial.println("R LONG PRESS");
            setUpTime();
          }
        }
      }
      lcd.clear();
      lcd.noBacklight();
      lcd.off();
    }
    time_t getUpTime() {
      return upTime;
    }
    time_t getDownTime() {
      return downTime;
    }
    int getMoveInterval() {
      return moveInterval;
    }
    String stringify() {
      return "upTime: " + formatTime(upTime) + ", downTime: " + formatTime(downTime);
    }
};
//---------------------------------------------------

int relayOn = -1;
Settings settings = Settings();

void setup() {
  Serial.begin(9600);
  digitalWrite(upRelayPin, RELAY_OFF);
  digitalWrite(downRelayPin, RELAY_OFF);

  lcd.init();
  setupRTC();

  pinMode(upRelayPin, OUTPUT); // nastavení pinu relé jako výstupu
  pinMode(downRelayPin, OUTPUT); // nastavení pinu relé jako výstupu
  pinMode(leftButtonPin, INPUT_PULLUP);
  pinMode(rightButtonPin, INPUT_PULLUP);
}


void setupRTC() {

  // Activate RTC module
  digitalWrite(DS1302_GND_PIN, LOW);
  pinMode(DS1302_GND_PIN, OUTPUT);

  digitalWrite(DS1302_VCC_PIN, HIGH);
  pinMode(DS1302_VCC_PIN, OUTPUT);

  lcd.print("RTC activated");
//
//  setTime(22,59,00,8,1,2020);
//  RTC.set(now());

  delay(500);

  lcd.clear();
  if (RTC.haltRTC())
    lcd.print("Clock stopped!");
  else
    lcd.print("Clock working.");

  lcd.setCursor(0, 1);
  if (RTC.writeEN())
    lcd.print("Write allowed.");
  else
    lcd.print("Write protected.");

  delay ( 2000 );

  // Setup time library
  lcd.clear();
  lcd.print("RTC Sync");
  setSyncProvider(RTC.get);          // the function to get the time from the RTC
  if (timeStatus() == timeSet)
    lcd.print(" Ok!");
  else
    lcd.print(" FAIL!");

  delay ( 2000 );

  lcd.clear();
}

bool alreadyMovedUp, alreadyMovedDown = false;

void loop() {
  bool l, r = false;
  int pressLength = checkButton(&l, &r);
  if (l & r) {
    Serial.println("SETTINGS");
    settings.configure();
  } else if (l) {
    if (pressLength > 1000) {
      lcd.on();
      lcd.backlight();
      lcd.clear();
      lcd.print(formatTime(now()));
      delay(2000);
      lcd.clear();
      lcd.noBacklight();
      lcd.off();
    } else {
      Serial.println("BLINDS DOWN");
      relaySwitch(downRelayPin);
    }
  } else if (r) {
    Serial.println("BLINDS UP");
    relaySwitch(upRelayPin);
  }


  if (!alreadyMovedUp && hour() == hour(settings.getUpTime()) && minute() == minute(settings.getUpTime())) {
    Serial.println("BLINDS UP AUTOMATICALLY");
    relaySwitch(upRelayPin);
    delay(settings.getMoveInterval() + 3000);
    turnRelayOff();
    alreadyMovedUp = true;
  } else if (!alreadyMovedDown && hour() == hour(settings.getDownTime()) && minute() == minute(settings.getDownTime())) {
    Serial.println("BLINDS DOWN AUTOMATICALLY");
    relaySwitch(downRelayPin);
    delay(settings.getMoveInterval());
    turnRelayOff();
    alreadyMovedDown = true;
  } else {
    if (minute() != minute(settings.getUpTime())) {
      alreadyMovedUp = false;
    }
    if (minute() != minute(settings.getDownTime())) {
      alreadyMovedDown = false;
    }
  }
  displayTime();

  if (timeStatus() != timeSet) {
    lcd.setCursor(0, 1);
    lcd.print(F("RTC ERROR: SYNC!"));
    delay(1000);
  }
}

time_t prevH, prevM;

void displayTime() {
  if (prevH == hour() && prevM == minute()) {
    return;
  }
  lcd.clear();
  lcd.print(String(hour()) + ":" + String(minute()));
}

bool isClose(int, int) {
  return true;
}

void relaySwitch(int relay_pin) {
  if (relayOn == relay_pin) {
    return;
  }
  if (relayOn != -1) {
    turnRelayOff();
    return;
  }
  digitalWrite(relay_pin, RELAY_ON);
  relayOn = relay_pin;
  delay(30);
}

void turnRelayOff() {
  digitalWrite(upRelayPin, RELAY_OFF);
  digitalWrite(downRelayPin, RELAY_OFF);
  relayOn = -1;
}
