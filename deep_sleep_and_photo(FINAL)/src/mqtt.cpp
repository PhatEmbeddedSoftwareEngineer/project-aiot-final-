#include "mqtt.h"
#include "sleep.h"

WiFiClient espClient;
PubSubClient client(espClient);


mqtt::mqtt()
{
    Serial.begin(baud);
}

void mqtt::setupWiFi()
{
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.mode(WIFI_STA); 
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) { 
        delay(500);
        Serial.print(">");
    }
    randomSeed(micros());
    Serial.printf("Da ket noi wifi thanh cong\n");

}


void mqtt::callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) { 
        Serial.print((char)payload[i]);
    }
}
void mqtt::reconnect()
{
    while (!client.connected())
    {
        Serial.print("Attempting MQTT connection...");
        String clientId = "ESP32Client-";
        clientId += String(random(0xffff), HEX);
        if (client.connect(clientId.c_str())) 
        {
            Serial.println("Connected");
            client.publish("/ThinkIOT/Publish", "Welcome");
            client.subscribe("/ThinkIOT/Subscribe"); 

        }
        else
        {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            delay(2000);
        }
    }
}

void mqtt::SetupMqtt()
{
    _mqtt.setupWiFi();
    client.setServer(mqtt_server, 1883);
    
    client.setCallback(_mqtt.callback); 

}

void mqtt::sendAll()
{
    // _mqtt.temp = _sensor.temp;
    // _mqtt.hum = _sensor.hum;

    // // Tạo đối tượng JSON
    // StaticJsonDocument<200> doc;
    // doc["temperature"] = random(10,100);
    // doc["humidity"] = random(10,100);

    // // Chuyển đổi đối tượng JSON thành chuỗi
    // String output;
    // serializeJson(doc, output);
    String output = "trigger";

    // Publish chuỗi JSON lên topic "/ThinkIOT/data"
    //client.publish("/iot/device/700625182ec8", output.c_str());
    client.publish("esp32/trigger/get", output.c_str());

}

void mqtt::runMqtt()
{
    if (!client.connected()) {
        reconnect();
    }
    client.loop();
    sendAll();
    //_sleep.complete=true;

    /*
    unsigned long now = millis();
    if (now - lastMsg > 2000)
    {
        lastMsg = now;
        sendAll();
    #if 0
        _mqtt.temp = random(25,28);
        String temp = String(_mqtt.temp, 2);
        client.publish("/temp", temp.c_str()); // publish temp topic /ThinkIOT/temp
        _mqtt.hum = random(60,65);
        String hum = String(_mqtt.hum, 1); 
        client.publish("/hum", hum.c_str());   // publish hum topic /ThinkIOT/hum
    #endif
    #if 0
        Serial.print("Temperature: ");
        Serial.println(_mqtt.temp);
        Serial.print("Humidity: ");
        Serial.println(_mqtt.hum);

    #endif
    }
    */

}


void mqtt::getMacAddress()
{
    Serial.printf("MAC ADDRESS:= %012llx\n",ESP.getEfuseMac());

}



mqtt _mqtt;