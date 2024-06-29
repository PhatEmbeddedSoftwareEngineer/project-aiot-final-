#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
extern "C" {
  #include "freertos/FreeRTOS.h"
  #include "freertos/timers.h"
}
#include <AsyncMqttClient.h>

// Change the credentials below, so your ESP32 connects to your router
#define WIFI_SSID "ecovi.iot"
#define WIFI_PASSWORD "sandbattery"
// Change the MQTT_HOST variable to your Raspberry Pi IP address, 
// so it connects to your Mosquitto MQTT broker
// 165.22.254.143
#define MQTT_HOST IPAddress(165, 22, 254, 143)
#define MQTT_PORT 1883

/**
 *   - username: econations-admin
  - password: admin123
 */

#define BROKER_USER "econations-admin"
#define BROKER_PASS "admin123"






class asyncMqtt
{
private:

public:
    static void connectToWifi();
    static void connectToMqtt();
    static void WiFiEvent(WiFiEvent_t event);
    static void onMqttConnect(bool sessionPresent);
    static void onMqttDisconnect(AsyncMqttClientDisconnectReason reason);
    static void onMqttSubscribe(uint16_t packetId, uint8_t qos);
    static void onMqttUnsubscribe(uint16_t packetId);
    static void onMqttPublish(uint16_t packetId);
    // You can modify this function to handle what happens when you receive a certain message in a specific topic
    static void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total);
    
    void setupAsyncMqtt();
    void loopAsyncMqtt();
};

extern asyncMqtt _asyncMqtt;
