#include <Arduino.h>
#include <WiFi.h>  
#include <PubSubClient.h>
#include <ArduinoJson.h>

extern WiFiClient espClient;
extern PubSubClient client;

class mqtt
{
private:
    const char* ssid = "Phat"; ///  wifi ssid 
    const char* password = "0937875855";
    const char* mqtt_server = "test.mosquitto.org";// mosquitto server url

    int baud=115200;
public:
    unsigned long lastMsg = 0;
    float temp = 0;
    float hum = 0;
    mqtt();

    void setupWiFi();
    static void callback(char* topic, byte* payload, unsigned int length);
    void reconnect();

    void SetupMqtt();

    void runMqtt();

    void getMacAddress();
    void sendAll();
};

extern mqtt _mqtt;