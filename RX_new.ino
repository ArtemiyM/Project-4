/*   Данный скетч делает следующее: передатчик (TX) отправляет массив
 *   данных, который генерируется согласно показаниям с кнопки и с 
 *   двух потенциомтеров. Приёмник (RX) получает массив, и записывает
 *   данные на реле, сервомашинку и генерирует ШИМ сигнал на транзистор.
    by AlexGyver 2016
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  //возможные номера труб

int count=0;
String out_date;
String code; 

void setup(){
  Serial.begin(9600); //открываем порт для связи с ПК
  radio.begin(); //активировать модуль
  radio.setAutoAck(1);         //режим подтверждения приёма, 1 вкл 0 выкл
  radio.setRetries(0,15);     //(время между попыткой достучаться, число попыток)
  radio.enableAckPayload();    //разрешить отсылку данных в ответ на входящий сигнал
  radio.setPayloadSize(32);     //размер пакета, в байтах

  radio.openReadingPipe(1,address[0]);      //хотим слушать трубу 0
  radio.setChannel(0x60);  //выбираем канал (в котором нет шумов!)

  radio.setPALevel (RF24_PA_MAX); //уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH, RF24_PA_MAX
  radio.setDataRate (RF24_250KBPS); //скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  //должна быть одинакова на приёмнике и передатчике!
  //при самой низкой скорости имеем самую высокую чувствительность и дальность!!
  
  radio.powerUp(); //начать работу
  radio.startListening();  //начинаем слушать эфир, мы приёмный модуль
  delay(3);
  Serial.println("CELL,GET,E2");
  count = Serial.readStringUntil(10).toInt() + 1;
  if(count > 2){
    Serial.println( (String)"ROW,SET," + count);
  }else{
    Serial.println( (String)"ROW,SET,2");
  } 
}

void loop() {
    byte pipeNo; 
    char gotByte;                      
    while( radio.available(&pipeNo)){    // слушаем эфир со всех труб
      radio.read( &gotByte, sizeof(gotByte) );         // чиатем входящий сигнал
      
      if(gotByte != 19){
//        Serial.print((char)gotByte);
        code = code + (char)gotByte;
      }else{
//        Serial.println();
//        Serial.println((String)"This code is String = " + code);
        Serial.println( (String) "DATA,DATE,TIME," + code + ",AUTOSCROLL_20");
        Serial.println( (String)"CELL,SET,E2," + count);
        count++;
        code = "";
      }
   }
   
   
   /*
    if(count == -20){
      delay(3);
      Serial.println("CELL,GET,A2");
      out_date = Serial.readStringUntil(10);
      Serial.println("PAUSELOGGING");
      Serial.println( (String)"SAVEWORKBOOKAS, " + out_date);
      Serial.println("FORCEEXCELQUIT");
    }*/
}


