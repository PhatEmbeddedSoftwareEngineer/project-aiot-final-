#include "asyncmqtt.h"


// Create objects to handle MQTT client
AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;


String temperatureString = "";      // Variable to hold the temperature reading
unsigned long previousMillis = 0;   // Stores last time temperature was published
const long interval = 5000;         // interval at which to publish sensor readings

void asyncMqtt::connectToWifi()
{
    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.printf(">");
        delay(500);
    }
    Serial.println("da ket noi thanh cong wifi");

}

void asyncMqtt::connectToMqtt()
{
    Serial.println("Connecting to MQTT...");
    mqttClient.connect();

}

void asyncMqtt::WiFiEvent(WiFiEvent_t event)
{
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch(event) {
        case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        connectToMqtt();
        break;
        case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
        xTimerStart(wifiReconnectTimer, 0);
        break;
    }
}

// Add more topics that want your ESP32 to be subscribed to

void asyncMqtt::onMqttConnect(bool sessionPresent)
{
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
    // ESP32 subscribed to esp32/led topic
    uint16_t packetIdSub = mqttClient.subscribe("esp32/led", 0);
    Serial.print("Subscribing at QoS 0, packetId: ");
    Serial.println(packetIdSub);
}
void asyncMqtt::onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
    Serial.println("Disconnected from MQTT.");
    if (WiFi.isConnected()) {
        xTimerStart(mqttReconnectTimer, 0);
    }
}
void asyncMqtt::onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
    Serial.println("Subscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
    Serial.print("  qos: ");
    Serial.println(qos);
}

void asyncMqtt::onMqttUnsubscribe(uint16_t packetId)
{
    Serial.println("Unsubscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}
void asyncMqtt::onMqttPublish(uint16_t packetId)
{
    Serial.println("Publish acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}
// You can modify this function to handle what happens when you receive a certain message in a specific topic
void asyncMqtt::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    String messageTemp;
    for (int i = 0; i < len; i++) {
        //Serial.print((char)payload[i]);
        messageTemp += (char)payload[i];
    }
    // Check if the MQTT message was received on topic esp32/led
    if (strcmp(topic, "esp32/led") == 0) {
        // If the LED is off turn it on (and vice-versa)
        // if (ledState == LOW) {
        // ledState = HIGH;
        // } else {
        // ledState = LOW;
        // }
        // // Set the LED with the ledState of the variable
        // digitalWrite(ledPin, ledState);
    }
    
    Serial.println("Publish received.");
    Serial.print("  message: ");
    Serial.println(messageTemp);
    Serial.print("  topic: ");
    Serial.println(topic);
    Serial.print("  qos: ");
    Serial.println(properties.qos);
    Serial.print("  dup: ");
    Serial.println(properties.dup);
    Serial.print("  retain: ");
    Serial.println(properties.retain);
    Serial.print("  len: ");
    Serial.println(len);
    Serial.print("  index: ");
    Serial.println(index);
    Serial.print("  total: ");
    Serial.println(total);
}
void asyncMqtt::setupAsyncMqtt()
{
    Serial.begin(115200);
    mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
    wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));
    WiFi.onEvent(WiFiEvent);

    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onSubscribe(onMqttSubscribe);
    mqttClient.onUnsubscribe(onMqttUnsubscribe);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.onPublish(onMqttPublish);
    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
    mqttClient.setCredentials(BROKER_USER, BROKER_PASS);
    
    connectToWifi();

}

void asyncMqtt::loopAsyncMqtt()
{
    unsigned long currentMillis = millis();
    // Every X number of seconds (interval = 5 seconds) 
    // it publishes a new MQTT message on topic esp32/temperature
    if (currentMillis - previousMillis >= interval) {
        // Save the last time a new reading was published
        previousMillis = currentMillis;
        // New temperature readings
        //sensors.requestTemperatures(); 
        // temperatureString = " " + String(sensors.getTempCByIndex(0)) + "C  " +  
        //                     String(sensors.getTempFByIndex(0)) + "F";     
        // Serial.println(temperatureString);
        // Publish an MQTT message on topic esp32/temperature with Celsius and Fahrenheit temperature readings
        float temp=random(25,27);
        float humidity = random(65,67);
        StaticJsonDocument<200> doc;
        doc["temperature"] = temp;
        doc["humidity"] = humidity;
        String output;
        serializeJson(doc, output);
        
        uint16_t packetIdPub2 = mqttClient.publish("/iot/device/700625182ec8", 2, true, output.c_str());
        Serial.print("Publishing on topic esp32/temperature at QoS 2, packetId: ");
        Serial.println(packetIdPub2);
    }
}
asyncMqtt _asyncMqtt;
