#include <Arduino.h>
#include <WiFi.h>
extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>

#define WIFI_SSID "Phat"
#define WIFI_PASSWORD "0937875855"

// BROKER 
// test.mosquitto.org has ip: 5.196.95.208​
#define MQTT_HOST "test.mosquitto.org" //MQTT BROKER IP ADDRESS
#define MQTT_PORT 1883

// define USER

#ifdef USER
    #define BROKER_USER "REPLACE_WITH_BROKER_USERNAME"
    #define BROKER_PASS "REPLACE_WITH_BROKER_PASSWORD"
#endif

//MQTT Subscribe Topics
#define MQTT_SUB_DIGITAL "esp32/digital/#"



class gpio
{
private:
    const int bauds=115200;
public:
    gpio();
    static void connectToWifi();
    static void connectToMqtt();
    static void WiFiEvent(WiFiEvent_t event);
    static void onMqttConnect(bool sessionPresent);
    static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
    static void onMqttSubscribe(uint16_t packetId, uint8_t qos);
    static void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    static void onMqttPublish(uint16_t packetId);
    void SetupGpioMqtt();

};

extern gpio _gpio;