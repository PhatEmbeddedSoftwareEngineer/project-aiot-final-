#include "sleep.h"
#include "photo.h"
// #include "mqtt.h"
#include "flash.h"
#include "dht11.h"
#include "asyncmqtt.h"


void setup() {
  _photo.setup();
  _flash.setupFlashPin();
#if 1
  for(int i=0;i<3;i++)
  {
    Serial.printf("MAC ADDRESS:= %012llx\n",ESP.getEfuseMac());
  }
  //_sensor.setupSensor();
  _asyncMqtt.setupAsyncMqtt();

#endif
  
  //_mqtt.SetupMqtt();
  //_sleep.setTimeWakeup();
  
}

void loop() {
  //_sensor.loopSensor();
  _asyncMqtt.loopAsyncMqtt();
  //delay(2000);
  // _mqtt.runMqtt();
  // delay(3000);
  

}
