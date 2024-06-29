#include "dht11.h"

DHT dht(DHTPIN, DHTTYPE);


sensor::sensor()
{
    Serial.begin(bauds);
}

void sensor::setupSensor()
{
    dht.begin();
    Serial.printf("da khoi tao xong ket noi voi cam bien\n");

}

void sensor::loopSensor()
{
    hum = dht.readHumidity();
    temp = dht.readTemperature();
    if (isnan(hum) || isnan(temp) ) {
        Serial.println(F("Failed to read from DHT sensor!"));
        return;
    }
#if 0
    Serial.printf("Humidity:= %f \t\t Temperature:= %f \n",h,t);
#endif

}



sensor _sensor;