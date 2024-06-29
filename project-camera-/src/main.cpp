#include <Arduino.h>
#include "photo.h"
#include "sleep.h"
#include "dht11.h"

#include "asyncmqtt.h"



void setup() {
#if 1
  
  //_mqtt.SetupMqtt();
  _photo.setupJpg();
  //_sleep.setupDeepSleep();
  //_sensor.setupSensor();
  //_mqtt.getMacAddress();

#endif
  _asyncMqtt.setupAsyncMqtt();
  
}

void loop() {
  // put your main code here, to run repeatedly:
#if 1
  _photo.runJpg();
  //_mqtt.runMqtt();
  //_sensor.loopSensor();
#endif
  _asyncMqtt.loopAsyncMqtt();

}

