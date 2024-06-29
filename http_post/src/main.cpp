/*
  author: Dario Arias
  
*/

// adding comment to test git configuration

#include <Arduino.h>
#include <WiFi.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_camera.h"

const char* ssid = "alterno.office";
const char* password = "sand.battery";

String serverName = "143.198.93.170";   // REPLACE WITH YOUR Raspberry Pi IP ADDRESS
//String serverName = "example.com";   // OR REPLACE WITH YOUR DOMAIN NAME

String serverPath = "/upload";     // The default serverPath should be upload.php

String deviceId = "2121";

const int serverPort = 80;
const int httpSuccess = 2;
const int sendButton = 14;

WiFiClient client;

// CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

const int timerInterval = 3000;    // time between each HTTP POST image
unsigned long previousMillis = 0;   // last time image was sent


String sendPhoto() ;

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); 
  Serial.begin(115200);

  WiFi.mode(WIFI_STA);
   Serial.println();
   Serial.print("Connecting to ");
   Serial.println(ssid);
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
     Serial.print(".");
    delay(500);
  }
   Serial.println();
   Serial.print("ESP32-CAM IP Address: ");
   Serial.println(WiFi.localIP());

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // init with high specs to pre-allocate larger buffers
  if(psramFound()){
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 10;  //0-63 lower number means higher quality
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_CIF;
    config.jpeg_quality = 12;  //0-63 lower number means higher quality
    config.fb_count = 1;
  }
  
  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
     Serial.printf("Camera init failed with error 0x%x", err);
    delay(1000);
    ESP.restart();
  }

  pinMode(sendButton, INPUT);
  
  pinMode(httpSuccess, OUTPUT);
  digitalWrite(httpSuccess, LOW);
  
//  sendPhoto(); 
}

void loop() {
  // TODO send a photo when a button is pressed
  unsigned long currentMillis = millis();
  int buttonState = digitalRead(sendButton); 
  
  if (currentMillis - previousMillis >= timerInterval) {
    digitalWrite(httpSuccess, LOW);
    previousMillis = currentMillis;
  }

  if(buttonState) {
    Serial.println("\nSending photo");
    sendPhoto();
    buttonState = 0;
  }
  
}
String sendPhoto() {
  String getBody;

  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if(!fb) {
    Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }

  Serial.println("Connecting to server: " + serverName);

  if (client.connect(serverName.c_str(), serverPort)) {
    Serial.println("Connection successful!");

    String boundary = "--------------------------" + String(micros(), HEX);
    String head = "--" + boundary + "\r\n" +
                  "Content-Disposition: form-data; name=\"file\"; filename=\"esp32-cam.jpg\"\r\n" +
                  "Content-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--" + boundary + "--\r\n";

    uint32_t imageLen = fb->len;
    uint32_t extraLen = head.length() + tail.length();
    uint32_t totalLen = imageLen + extraLen;

    client.println("POST " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName);
    client.println("Content-Type: multipart/form-data; boundary=" + boundary);
    client.println("Content-Length: " + String(totalLen));
    client.println("deviceid: " + deviceId);
    client.println();
    client.print(head);

    size_t fbLen = fb->len;
    uint8_t *fbBuf = fb->buf;

    // Send image data
    client.write(fbBuf, fbLen);

    // Send end of multipart/form-data
    client.print(tail);

    esp_camera_fb_return(fb);

    // Wait for server response
    unsigned long timeout = millis();
    while (client.available() == 0) {
      if (millis() - timeout > 5000) {
        Serial.println(">>> Client Timeout !");
        client.stop();
        return "";
      }
    }

    // Read response from server
    while (client.available()) {
      char c = client.read();
      getBody += c;
    }

    client.stop();

    Serial.println("Response from server:");
    Serial.println(getBody);

    digitalWrite(httpSuccess, HIGH);
    previousMillis = millis();
  } else {
    Serial.println("Connection to " + serverName + " failed.");
    getBody = "Connection failed";
    digitalWrite(httpSuccess, LOW);
  }

  return getBody;
}

/*
String sendPhoto() {
  String getAll;
  String getBody;

  camera_fb_t * fb = NULL;
  fb = esp_camera_fb_get();
  if(!fb) {
     Serial.println("Camera capture failed");
    delay(1000);
    ESP.restart();
  }
  
   Serial.println("Connecting to server: " + serverName);

  if (client.connect(serverName.c_str(), serverPort)) {
     Serial.println("Connection successful!");    
    String head = "--IOTobjectCounter\r\nContent-Disposition: form-data; name=\"name\"; filename=\"esp32-cam.jpg\"\r\nContent-Type: image/jpeg\r\n\r\n";
    String tail = "\r\n--IOTobjectCounter--\r\n";

    uint32_t imageLen = fb->len;
    uint32_t extraLen = head.length() + tail.length();
    uint32_t totalLen = imageLen + extraLen;
  
    client.println("POST " + serverPath + " HTTP/1.1");
    client.println("Host: " + serverName);
    //client.println("x-device-id: " + deviceId);
    client.println("deviceid: " + deviceId);
    client.println("Content-Length: " + String(totalLen));
    client.println("Content-Type: multipart/form-data; boundary=IOTobjectCounter");
    client.println();
    client.print(head);

     
  
    uint8_t *fbBuf = fb->buf;
    size_t fbLen = fb->len;
   
    // data
    for (size_t n = 0; n < fbLen; n += 1024) {
      if (n + 1024 < fbLen) {
        client.write(fbBuf, 1024);
        fbBuf += 1024;
      }
      else if (fbLen % 1024 > 0) {
        size_t remainder = fbLen % 1024;
        client.write(fbBuf, remainder);
      }
    }   

    client.print(tail);

    // simple get request.
//    client.println("GET /statusz HTTP/1.1");
//    client.println("Host: " + serverName);
//    client.println();

     Serial.println("Request completed");
    
    esp_camera_fb_return(fb);
    
    int timoutTimer = 10000;
    long startTimer = millis();
    boolean state = false;
    boolean charsRead = false;
    
    while ((startTimer + timoutTimer) > millis()) {
       Serial.print(".");
      delay(100);    
      
      // while there are available chars
      while (client.available()) {
        char c = client.read();
        if (c == '\n') {
          if (getAll.length() == 0) { 
            state = true; 
          }
          getAll = "";
        }
        else if (c != '\r') { 
          getAll += String(c); 
        }
        
        if (state) { 
          getBody += String(c); 
        }

        startTimer = millis();
      }
      
      // if you read all characters don't try again.
      if (getBody.length() > 0) {
        charsRead = true;
        break;
      }
    }
    
    // TODO 
    // Turn a led when the message is sent succesfully
    if(!charsRead) {
      Serial.println("\nClient response timeout");
      digitalWrite(httpSuccess, LOW);
    }
    else {
      Serial.println();
      client.stop();
      Serial.println(getBody + '\n');
      digitalWrite(httpSuccess, HIGH);
      previousMillis = millis();
    }

  }
  else {
    getBody = "Connection to " + serverName +  " failed.";
     Serial.println(getBody);

  }
  return getBody;
}
*/