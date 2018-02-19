#include <hiduniversal.h>               //Add to Oleg Mazurov code to Bar Code Scanner
#include <hidboot.h>  
#include <SPI.h>          // библиотека для работы с шиной SPI
#include "nRF24L01.h"     // библиотека радиомодуля
#include "RF24.h"         // ещё библиотека радиомодуля
RF24 radio(6, 7); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; //возможные номера труб
//char counter;

USB     Usb;
//USBHub     Hub(&Usb);                                          //I enable this line
HIDUniversal      Hid(&Usb);                                  //Add this line so that the barcode scanner will be recognized, I use "Hid" below 
HIDBoot<HID_PROTOCOL_KEYBOARD>    Keyboard(&Usb);
 
class KbdRptParser : public KeyboardReportParser
{
        void PrintKey(uint8_t mod, uint8_t key);             // Add this line to print character in ASCII
protected:
  virtual void OnKeyDown  (uint8_t mod, uint8_t key);
  virtual void OnKeyPressed(uint8_t key);
};
 
void KbdRptParser::OnKeyDown(uint8_t mod, uint8_t key)  
{
    uint8_t c = OemToAscii(mod, key);
    
   if (c)
        OnKeyPressed(c);
}
 
/* what to do when symbol arrives */
void KbdRptParser::OnKeyPressed(uint8_t key)  
{
static uint32_t next_time = 0;      //watchdog
static uint8_t current_cursor = 0;  //tracks current cursor position 

    Serial.print((char) key);
    radio.write(&key, sizeof(key));
    delay(10);

  //counter=(char)key;
};

KbdRptParser Prs;
 
void setup()
{
  Serial.begin( 9600 );
  Serial.println("Start");
  
  if (Usb.Init() == -1) {
    Serial.println("OSC did not start.");
  }
  delay( 200 );
 
  Hid.SetReportParser(0, (HIDReportParser*)&Prs);        //Here I change  "Keyboard" for "Hid"
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0, 15);    //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openWritingPipe(address[0]);   //мы - труба 0, открываем канал для передачи данных
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!

  radio.powerUp(); //начать работу
  radio.stopListening();  //не слушаем радиоэфир, мы передатчик
    
}
 
void loop()
{
  Usb.Task();
}
