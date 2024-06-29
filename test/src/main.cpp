#include <Arduino.h>
#include "mqtt.h"
#include "photo.h"
#include "sleep.h"

void setup() {
#if 1
  _mqtt.SetupMqtt();
  _photo.setupJpg();

#endif
  _sleep.setTimeWakeup();
}

void loop() {
  // put your main code here, to run repeatedly:
#if 0
  _mqtt.runMqtt();
  _photo.runJpg();
#endif
  _sleep.startSleep();
  _sleep.fcnAfterWakeup();
  _sleep.setTimeWakeup();
}

