// připoj knihovny
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS1302RTC.h>
#include <Wire.h>
// inicializace DS1302
// nastavení pinů: CE, IO, CLK
DS1302RTC RTC (8, 9, 10);
// Inicializovat LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup () {
   /*  Uncomment below only the very first time you run the program. 
    Then comment out again and re upload 
 setTime(13, 25, 00, 22, 12, 2017);   // 23h31m30s on 3Feb2009
 RTC.set(now());              //set the RTC from the system time
 RTC.haltRTC(true);     
 RTC.haltRTC(false);  */ // This starts the clock.  OR it will reset every 5 minutes
  Serial.begin(9600); // spustit sériový monitor na 9600 bps
  lcd.init(); // inicializace lcd
  lcd.backlight(); // zapnout podsvícení
  // aktivace RTC
  lcd.print("RTC aktivovan");
  delay(500);
  // kontrola oscilace
  lcd.clear();
  if (RTC.haltRTC())    // Check clock oscillation  
  {
  Serial.print("Clock stopped!");
  delay(1000);
  }
  else
  {
   Serial.println("Clock is working.");
   delay(1000);
  }
  if (RTC.writeEN()){Serial.println("Write allowed.");}   // Check write-protection
  else  { Serial.println("Write protected."); }

  Serial.print("RTC Sync in Operation");      // Setup Time library  
  Serial.println(" . . . ");
  setSyncProvider(RTC.get); // the function to get the time from the RTC
  if(timeStatus() == timeSet) { Serial.println(" Ok!"); }
  else { Serial.println(" FAIL!"); } 
}
void loop() {
  // zobrazit čas na displeji
  lcd.setCursor(3, 0);
  print2digits(hour());
  lcd.print(":");
  print2digits(minute());
  lcd.print(":");
  print2digits(second());
  // zobrazit den v týdnu
  lcd.setCursor(0, 1);
  lcd.print(dayShortStr(weekday()));
  // zobrazit datum
  lcd.setCursor(5, 1);
  lcd.print(" ");
  print2digits(day());
  lcd.print(".");
  print2digits(month());
  lcd.print(".");
  lcd.print(year());
  // pozor!
  if (timeStatus() != timeSet) {
    lcd.setCursor(0, 1);
    lcd.print(F("CHYBA RTC: SYNC!"));
  }
  delay ( 1000 ); // počkej přibližně 1 sec
}
void print2digits(int number ) {
  // přidaní "0" u čísel do 10
  if (number >= 0 && number < 10) {
    lcd.write('0');
  }
  lcd.print(number);
}
