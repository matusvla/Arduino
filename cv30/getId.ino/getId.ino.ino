// RFID vstupní ochranný systém -> getId.ino
// Umísti klíč RFID tagu do indukční zóny modulu RFID.
// Uvidíš následující hodnoty vytištěné na seriálovém monitoru: // Email:laskarduino@gmail.com
// Web:laskarduino.cz /*/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/\/*/
#include "rfid1.h"
RFID1 rfid; // vytvoříme objekt typu RFID1
uchar serNum[5]; // pole pro ukládání ID
void setup() {
Serial.begin(9600);// spustit sériový monitor //rfid.begin(IRQ_PIN,SCK_PIN,MOSI_PIN,MISO_PIN,NSS_PIN,RST_PIN) -IRQ_PIN není potřeba
zapojovat
rfid.begin(7, 5, 4, 3, 6, 2);
          delay(100);
          rfid.init();
}
void loop() {
//inicializace RFID
}
uchar status;
uchar str[MAX_LEN];
// Hledání čipu
status = rfid.request(PICC_REQIDL, str); if (status != MI_OK) {
return;
}
// Ukázat druh čipu
rfid.showCardType(str);
//Předejdi konfliktu a vrať 4 bajty sériového čísla karty status = rfid.anticoll(str);
if (status == MI_OK) {
Serial.print("The card's number is: "); memcpy(serNum, str, 5); rfid.showCardID(serNum); Serial.println();
Serial.println();
}
delay(500);
rfid.halt();
// uspat modul
