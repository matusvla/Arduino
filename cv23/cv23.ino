// Hodiny reálného času
// Teď by jsi na displeji I2C LCD1602 měl vidět správný datum a čas.
// Email:laskarduino@gmail.com
// Web:laskarduino.cz
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/
// připoj knihovny
#include <LiquidCrystal_I2C.h>
#include <Time.h>
#include <TimeLib.h>
#include <DS1302RTC.h>
#include <Wire.h>
// inicializace DS1302
// nastavení pinů: CE, IO, CLK
DS1302RTC RTC (10, 11, 12);
// Inicializovat LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);
void setup () {
  Serial.begin(9600); // spustit sériový monitor na 9600 bps
  lcd.init(); // inicializace lcd
  lcd.backlight(); // zapnout podsvícení
  // aktivace RTC
  lcd.print("RTC aktivovan");
  delay(500);
  // kontrola oscilace
  lcd.clear();
  if (RTC.haltRTC())
    lcd.print("Hodiny zastaveny");
  else
    lcd.print("Hodiny pracujou");
  // kontrola write-protection
  lcd.setCursor(0, 1);
  if (RTC.writeEN())
    lcd.print("Zapis povolen");
  else
    lcd.print("Zapis zakazan");
  delay ( 1000 );
  // nastavení knihovny
  lcd.clear();
  lcd.print("RTC Sync");
  setSyncProvider(RTC.get); // získat čas z RTC
  if (timeStatus() == timeSet)
    lcd.print("OK!");
  else
    lcd.print("SELHALO!");
  delay ( 1000 );
  lcd.clear();
  // nastavit čas a datum
  // (hod,min,sec,den,mesic,rok);
  setTime(22,58,00,26,12,2019);
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
