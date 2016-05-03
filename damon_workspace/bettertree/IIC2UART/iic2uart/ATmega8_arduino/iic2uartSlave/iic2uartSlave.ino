#include <Wire.h>
void receiveEvent(); 
void requestEvent();

void setup()
{
  Wire.begin(8);   //加入iic 地址为8
  Wire.onReceive(receiveEvent);//注册接受函数
  Wire.onRequest(requestEvent);//注册主机通知从机上传数据
  Serial.begin(38400);
  Serial.print("start \n");
}
 
void loop()
{
  delay(100);
}

void receiveEvent(int howMany)
{
  
  while (0 < Wire.available()) { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
  } 
  
}

void requestEvent()
{
  // send data only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    char c = Serial.read();
    //send it to i2c BUS
    Wire.write(c);
  }
}

