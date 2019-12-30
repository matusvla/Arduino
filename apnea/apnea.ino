// I2C LCD displej
// Teď by jsi měl na svém displeji vidět běžící text:“Laskarduino” a "Zdravim bastlire!".
// Email:laskarduino@gmail.com
// Web:laskarduino.cz
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/
// připoj knihovny
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS1302RTC.h>
#include <Wire.h>

#define uint8 unsigned char
#define button1 2 // button 1 pin number
#define button2 3 // button 2 pin number
#define led1 6 // led 1 pin number
#define led2 7 // led 2 pin number
#define buzzer 8 // buzzer pin number

// Setings class definition

String formatTime(int n) {
  int sec = second(n);
  int m = minute(n);
  String s;
  if (m < 10) {
    s += '0';
  }
  s  += String(m);
  s += ":";
  if (sec < 10) {
    s += '0';
  }
  s += String(sec);
  return s;
}

class Settings
{
  private:
    time_t start_time;
    time_t raise_time;
    time_t breathe_time;
    int rep;
  public:
    Settings() {
      this->start_time = 0;
      this->raise_time = 0;
      this->rep = 8;
    }

    String getSettingsString(bool hold) {
      String s;
      if (hold) {
        s = formatTime(this->start_time) + " ";
        s += formatTime(this->raise_time) + " ";
        s += getRep();
      } else {
        s = formatTime(this->breathe_time);
      }
      return s;
    }
    int getRep()  {
      return this->rep;
    }
    time_t getStartTime() {
      return this->start_time;
    }
    time_t getRaiseTime() {
      return this->raise_time;
    }
    time_t getBreatheTime() {
      return this->breathe_time;
    }
    void changeStartTime(int by_value) {
      if (int(start_time) + by_value >= 0) {
        start_time += by_value;
      }
    }
    int changeRaiseTime(int by_value) {
      if (int(raise_time) + by_value >= 0) {
        raise_time += by_value;
      }
    }
    int changeBreatheTime(int by_value) {
      if (int(breathe_time) + by_value >= 0) {
        breathe_time += by_value;
      }
    }
    int changeRep(int by_value)  {
      if (rep + by_value > 0) {
        rep += by_value;
      }
    }
};

// Constants

Settings settings = Settings();
time_t * intervals;
uint8 b1State, b2State = 0;

// Display init
LiquidCrystal_I2C lcd(0x27, 16, 2);
// DS1302 Init
// pin setup: CE, IO, CLK
DS1302RTC RTC (10, 11, 12);

// Setup func

void setup() {
  Serial.begin(9600);
  lcd.init();
//  lcd.backlight(); // zapnout podsvícení
  pinMode(button1, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(buzzer, OUTPUT);
  verifyOutputConnections();
  setupRTC();
}

void verifyOutputConnections() {
  digitalWrite(led1, 1);
  digitalWrite(led2, 1);
  digitalWrite(buzzer, 1);
  delay(200);
  digitalWrite(led1, 0);
  digitalWrite(led2, 0);
  digitalWrite(buzzer, 0);
}

void setupRTC() {
  // aktivace RTC
  lcd.print("RTC active");
  delay(500);
  // kontrola oscilace
  lcd.clear();
  if (RTC.haltRTC())
    lcd.print("Clock stopped");
  else
    lcd.print("Clock working");
  // kontrola write-protection
  lcd.setCursor(0, 1);
  if (RTC.writeEN())
    lcd.print("Writing allowed");
  else
    lcd.print("Writing forbidden");
  delay ( 1000 );
  // nastavení knihovny
  lcd.clear();
  lcd.print("RTC Sync ");
  setSyncProvider(RTC.get); // získat čas z RTC
  if (timeStatus() == timeSet)
    lcd.print("OK!");
  else
    lcd.print("FAIL!");
  delay ( 1000 );
  lcd.clear();
  // set Time:
  // (h,m,s,D,M,Y);
  //  setTime(22,58,00,26,12,2019);
}

// Main loop

void loop() {
  changeSettings();
  intervals = countIntervals();
  runTimer(intervals);
  delete[] intervals;
}

time_t * countIntervals() {
  int rep = settings.getRep();
  time_t holdTime = settings.getStartTime();
  time_t raiseTime = settings.getRaiseTime();
  time_t breatheTime = settings.getBreatheTime();
  time_t * intervals = new time_t[2 * rep];
  for (int i  = 0; i < rep; ++i) {
    intervals[2 * i] = breatheTime;
    intervals[2 * i + 1] = holdTime + i * raiseTime;
  }
  return intervals;
}

void changeSettings() {
  setupline(0, "START RAISE REP");
  setupline(1, settings.getSettingsString(true));
  Serial.println("ss" + settings.getSettingsString(true));
  while (1) {
    b1State = digitalRead(button1);
    b2State = digitalRead(button2);
    if (b1State == 0 && b2State == 0) {
      waitForButtonRelease();
      break;
    }
    if (b1State == 0) {
      settings.changeStartTime(5);
      setupline(1, settings.getSettingsString(true));
      delay(200);
      Serial.println(settings.getSettingsString(true));
    }
    if (b2State == 0) {
      settings.changeStartTime(-5);
      setupline(1, settings.getSettingsString(true));
      delay(200);
    }
  }
  while (1) {
    b1State = digitalRead(button1);
    b2State = digitalRead(button2);
    if (b1State == 0 && b2State == 0) {
      waitForButtonRelease();
      break;
    }
    if (b1State == 0) {
      settings.changeRaiseTime(5);
      setupline(1, settings.getSettingsString(true));
      delay(200);
    }
    if (b2State == 0) {
      settings.changeRaiseTime(-5);
      setupline(1, settings.getSettingsString(true));
      delay(200);
    }
  }
  while (1) {
    b1State = digitalRead(button1);
    b2State = digitalRead(button2);
    if (b1State == 0 && b2State == 0) {
      waitForButtonRelease();
      break;
    }
    if (b1State == 0) {
      settings.changeRep(1);
      setupline(1, settings.getSettingsString(true));
      delay(200);
    }
    if (b2State == 0) {
      settings.changeRep(-1);
      setupline(1, settings.getSettingsString(true));
      delay(200);
    }
  }
  setupline(0, "BREATHE");
  setupline(1, settings.getSettingsString(false));
  while (1) {
    b1State = digitalRead(button1);
    b2State = digitalRead(button2);
    if (b1State == 0 && b2State == 0) {
      waitForButtonRelease();
      break;
    }
    if (b1State == 0) {
      settings.changeBreatheTime(5);
      setupline(1, settings.getSettingsString(false));
      delay(200);
    }
    if (b2State == 0) {
      settings.changeBreatheTime(-5);
      setupline(1, settings.getSettingsString(false));
      delay(200);
    }
  }
}

void runTimer(time_t intervals[]) {
  setupline(0, "PRESS TO START");
  setupline(1, formatTime(intervals[0]));
  String prev_time, curr_time;
  while (1) {
    b1State = digitalRead(button1);
    b2State = digitalRead(button2);
    if (b1State == 0 && b2State == 0) {
      break;
    }
    if (b1State == 0) {
      for (int i = 0; i < 2 * settings.getRep(); i++) {
        time_t stopwatch_start_time = now();
        time_t want_time = intervals[i];
        if (i % 2 == 1) {
          setupline(0, "HOLD:");
          switchLed(2);
        } else {
          setupline(0, "BREATHE:");
          switchLed(1);
        }
        setupline(1, "");
        while (1) {
          time_t elapsed = now() - stopwatch_start_time;
          time_t remaining = want_time - elapsed;
          curr_time = formatTime(remaining);
          if (prev_time != curr_time) {
            setupline(1, curr_time);
            prev_time = curr_time;
          }
          if (remaining <= 0) {
            digitalWrite(buzzer, HIGH);
            delay(500); // počkat 1ms
            digitalWrite(buzzer, LOW);
            break;
          }
          b1State = digitalRead(button1);
          b2State = digitalRead(button2);
          if (b1State == 0 && b2State == 0) {
            waitForButtonRelease();
            return;
          } else if (b2State == 0) {
            time_t pauseStart = now();
            waitForButtonRelease();
            while (b2State != 0) {
              b2State = digitalRead(button2);
            }
            waitForButtonRelease();
            stopwatch_start_time += now() - pauseStart;
          }
        }
        //        // todo improve the stopping and printing
        //        b2State = digitalRead(button2);
      }
      switchLedOff();
      return;
    }
  }
  waitForButtonRelease();
}

void setupline(int n, String contents) {
  lcd.setCursor(0, n);
  lcd.print("                           ");
  lcd.setCursor(0, n);
  lcd.print(contents);
}

void waitForButtonRelease() {
  while (b1State == 0 || b2State == 0) {
    b1State = digitalRead(button1);
    b2State = digitalRead(button2);
  }
}

void switchLedOff() {
  digitalWrite(led1, 0);
  digitalWrite(led2, 0);
}

void switchLed(int ledIndex) {
  if (ledIndex == 1) {
    digitalWrite(led1, 1);
    digitalWrite(led2, 0);
  } else if (ledIndex == 2) {
    digitalWrite(led1, 0);
    digitalWrite(led2, 1);
  }
}
