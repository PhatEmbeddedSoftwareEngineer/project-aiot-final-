#include "gpio.h"

AsyncMqttClient mqttClient;
TimerHandle_t mqttReconnectTimer;
TimerHandle_t wifiReconnectTimer;



gpio::gpio()
{
    Serial.begin(bauds);
}

void gpio::connectToWifi()
{

    Serial.println("Connecting to Wi-Fi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    int cnt=0;
    while(WiFi.status() != WL_CONNECTED)
    {
        Serial.printf(">");
        delay(500);
        cnt++;
        if(cnt==6)
            esp_restart();
    }
    Serial.println("WiFi connected");

}
void gpio::connectToMqtt()
{
    Serial.println("Connecting to MQTT...");
    mqttClient.connect();
}

void gpio::WiFiEvent(WiFiEvent_t event)
{
    Serial.printf("[WiFi-event] event: %d\n", event);
    switch(event) {
        case ARDUINO_EVENT_WIFI_STA_GOT_IP:
            Serial.println("WiFi connected");
            Serial.println("IP address: ");
            Serial.println(WiFi.localIP());
            connectToMqtt();
        break;
        case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
            Serial.println("WiFi lost connection");
            xTimerStop(mqttReconnectTimer, 0); // ensure we don't reconnect to MQTT while reconnecting to Wi-Fi
            xTimerStart(wifiReconnectTimer, 0);
        break;
    }
}

void gpio::onMqttConnect(bool sessionPresent)
{
    Serial.println("Connected to MQTT.");
    Serial.print("Session present: ");
    Serial.println(sessionPresent);
    //Subscribe to topics
    // Subscribe to topic MQTT_SUB_DIGITAL when it connects to the broker
    uint16_t packetIdSub1 = mqttClient.subscribe(MQTT_SUB_DIGITAL, 2);
    Serial.print("Subscribing at QoS 2, packetId: ");
    Serial.println(packetIdSub1);
}

void gpio::onMqttDisconnect(AsyncMqttClientDisconnectReason reason)
{
    Serial.println("Disconnected from MQTT.");

    if (WiFi.isConnected()) {
        xTimerStart(mqttReconnectTimer, 0);
    }
}

void gpio::onMqttSubscribe(uint16_t packetId, uint8_t qos)
{
    Serial.println("Subscribe acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
    Serial.print("  qos: ");
    Serial.println(qos);
}

void gpio::onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total)
{
    // Do whatever you want when you receive a message
    // Save the message in a variable
    String receivedMessage;
    for (int i = 0; i < len; i++) {
        Serial.println((char)payload[i]);
        receivedMessage += (char)payload[i];
    }

    // Save topic in a String variable
    String receivedTopic = String(topic);  
    Serial.print("Received Topic: ");
    Serial.println(receivedTopic);

    // Check which GPIO we want to control
    int stringLen = receivedTopic.length();
    // Get the index of the last slash
    int lastSlash = receivedTopic.lastIndexOf("/");
    // Get the GPIO number (it's after the last slash "/")
    // esp/digital/GPIO
    // lastSlash+1 get number after "/"
    String gpio = receivedTopic.substring(lastSlash+1, stringLen);
    Serial.print("DIGITAL GPIO: ");
    Serial.println(gpio);
    Serial.print("STATE: ");
    Serial.println(receivedMessage);

    // Check if it is DIGITAL
    if (receivedTopic.indexOf("digital") > 0)
    {
        //Set the specified GPIO as output
        pinMode(gpio.toInt(), OUTPUT);
        static int flag=0;
        //Control the GPIO
        if (receivedMessage == "1"){
            flag++;
            digitalWrite(gpio.toInt(), HIGH);
            if(flag==2)
            {
                digitalWrite(gpio.toInt(), LOW);
                flag=0;
            }
        }
        else
        {
            digitalWrite(gpio.toInt(), LOW);
        }
    }
    Serial.println("Publish received.");
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
void gpio::onMqttPublish(uint16_t packetId) 
{
    Serial.println("Publish acknowledged.");
    Serial.print("  packetId: ");
    Serial.println(packetId);
}


void gpio::SetupGpioMqtt()
{
    mqttReconnectTimer = xTimerCreate("mqttTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToMqtt));
    wifiReconnectTimer = xTimerCreate("wifiTimer", pdMS_TO_TICKS(2000), pdFALSE, (void*)0, reinterpret_cast<TimerCallbackFunction_t>(connectToWifi));

    WiFi.onEvent(WiFiEvent);

    mqttClient.onConnect(onMqttConnect);
    mqttClient.onDisconnect(onMqttDisconnect);
    mqttClient.onSubscribe(onMqttSubscribe);
    mqttClient.onMessage(onMqttMessage);
    mqttClient.onPublish(onMqttPublish);

    mqttClient.setServer(MQTT_HOST, MQTT_PORT);
#ifdef  USER
    mqttClient.setCredentials(BROKER_USER, BROKER_PASS);
#endif 
    connectToWifi();
}

gpio _gpio;