int i=0, count=0;
String out_date;

void setup(){
  delay(1000);
  Serial.begin(9600);
  delay(3);
  Serial.println("CELL,GET,E2");
  count = Serial.readStringUntil(10).toInt() + 1;
  if(count > 2){
    Serial.println( (String)"ROW,SET," + count);
  }else{
    Serial.println( (String)"ROW,SET,2");
  } 
}

void loop(){
//  if(i%10 == 0){
    Serial.println( (String) "DATA,DATE,TIME," + i + ",AUTOSCROLL_20");
    Serial.println( (String)"CELL,SET,E2," + count);
    count++;
//  }
  if(count == 15){
    delay(3);
    Serial.println("CELL,GET,A2");
    out_date = Serial.readStringUntil(10);
    Serial.println(out_date);
    Serial.println("PAUSELOGGING");
    Serial.println( (String)"SAVEWORKBOOKAS, " + out_date);
    Serial.println("FORCEEXCELQUIT");
  }
  i++;
  delay(100);
}
