#include <Arduino.h>
#include "DHT.h"

#define DHTPIN 3 //uorxd
#define DHTTYPE DHT11   // DHT 11



class sensor
{
private:
    const int bauds=115200;
public:
    float hum;
    float temp;
    sensor();
    void setupSensor();
    void loopSensor();
};

extern sensor _sensor;
