#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10); // "создать" модуль на пинах 9 и 10 Для Уно
//RF24 radio(9,53); // для Меги

byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  //возможные номера труб

int count=0, i=0;
boolean condition = false, condition2 = false;
String out_date;
String code;
String pre[8];

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
  Serial.println("CELL,GET,E2"); //смотрим кол-во строк
  count = Serial.readStringUntil(10).toInt() + 1; //присваеваем это кол-во строк
  Serial.println( (String)"ROW,SET," + count); //назначаем начало записи

  delay(3);
  Serial.println("CELL,GET,E4");
  pre[0] = Serial.readStringUntil(10).toInt();
  delay(3);
  Serial.println("CELL,GET,E5");
  pre[1] = Serial.readStringUntil(10).toInt();
  delay(3);
  Serial.println("CELL,GET,E6");
  pre[2] = Serial.readStringUntil(10).toInt();
  delay(3);
  Serial.println("CELL,GET,E7");
  pre[3] = Serial.readStringUntil(10).toInt();
  delay(3);
  Serial.println("CELL,GET,E8");
  pre[4] = Serial.readStringUntil(10).toInt();
  delay(3);
  Serial.println("CELL,GET,E9");
  pre[5] = Serial.readStringUntil(10).toInt();
  delay(3);
  Serial.println("CELL,GET,E10");
  pre[6] = Serial.readStringUntil(10).toInt();
  delay(3);
  Serial.println("CELL,GET,E11");
  pre[7] = Serial.readStringUntil(10).toInt();
}

void loop() {
    byte pipeNo; 
    char gotByte;                      
    while( radio.available(&pipeNo)){    // слушаем эфир со всех труб
      radio.read( &gotByte, sizeof(gotByte) );         // чиатем входящий сигнал
      if(gotByte != 19){
          code = code + (char)gotByte;
      }
      
      for(i=0;i<8;i++){
        if(code == pre[i]){
          condition = true;
          condition2 = true;
        }
      }
      
      if(gotByte == 19){
        if(condition == true){
          Serial.print((String) "DATA," + code + ",");
          condition = false;
          code = "";
        }else if(condition2 == true){
          Serial.println((String)code);
          Serial.println((String)"CELL,SET,E2," + count);
          condition2 = false;
          code = "";
          count++;
        }else{
          Serial.print((String)"DATA," + ",");
          Serial.println((String)code);
          Serial.println((String)"CELL,SET,E2," + count);
          code = "";
          count++;
        }
     }
   }
}


