// RFID vstupní ochranný systém -> rfid.ino
// Teď projeď klíč RFID tagu přes RFID modul. Pokud je heslo správné, LCD zobrazí „ID:
// D0E7C280“ „Zdravím Bastlíře“. O dvě vteřiny později se na dipleji zobrazí „Vítej!“.
// Pokud je heslo nesprávné, LCD zobrazí: „Ahoj cizinče!“, a poté opět po dvou vteřinách „Vítej!“.. // Email:laskarduino@gmail.com
// Web:laskarduino.cz
/*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/
#include"rfid.h"
#include <LiquidCrystal_I2C.h> #include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); RFID rfid;
#define relayPin 8
uchar serNum[5];
void setup() {
  lcd.init();
  // vytvořeme objekt typu RFID1 // číslo pinu relé
  // pole pro ukladaní ID
  lcd.backlight();
  Serial.begin(9600);
  //rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,NSS_PIN,RST_PIN) - IRQ_PIN není potřeba
  zapojovat
  rfid.begin(7, 5, 4, 3, 6, 2);
  delay(100);
  rfid.init(); // inicializace RFID pinMode(relayPin, OUTPUT); // nastavení pinu relé jako výstupu digitalWrite(relayPin,LOW); // zapnut relé
  lcd.setCursor(0, 0); lcd.print(" Vitej!");
  delay(2000);
}

void loop() {
  uchar status;
  uchar str[MAX_LEN];
  // Hledání čipu
  status = rfid.request(PICC_REQIDL, str); if (status != MI_OK) {
    return;
  }
  // Ukázat druh čipu
  rfid.showCardType(str);
  // Předejdi problému a vrať 4 bajty sériového čísla karty. status = rfid.anticoll(str);
  if (status == MI_OK) {
    lcd.setCursor(0, 0);
    lcd.print(" ID: ");
    memcpy(serNum, str, 5); rfid.showCardID(serNum); // Ukázat ID čipu
    // Jestli ID čipu je 4BE6D13B, zapni relé
    uchar* id = serNum;
    if ( id[0] == 0x4B && id[1] == 0xE6 && id[2] == 0xD1 && id[3] == 0x3B ) {
    }
    digitalWrite(relayPin, HIGH); lcd.setCursor(0, 1); lcd.print(" Ahoj Kosto! "); delay(2000);
    lcd.clear(); digitalWrite(relayPin, LOW);
    // Jestli ID čipu je D0E7C280, zapni relé
    else if (id[0] == 0xD0 && id[1] == 0xE7 && id[2] == 0xC2 && id[3] == 0x80) { // D0-E7-C2-80
      digitalWrite(relayPin, HIGH); lcd.setCursor(0, 1); lcd.print("Zdravim Bastlire"); delay(2000);
      lcd.clear();
      digitalWrite(relayPin, LOW);
    } else {
      lcd.setCursor(0, 1); lcd.print(" Ahoj cizinec!"); delay(2000);
      lcd.clear();
    }
  }
  lcd.setCursor(0, 0); lcd.print(" Vitej! "); delay(2000);
  rfid.halt(); // uspat modul
}
