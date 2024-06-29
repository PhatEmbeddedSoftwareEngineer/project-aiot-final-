#include <Arduino.h>
#include "esp_http_server.h"
#include "esp_timer.h"
#include "esp_camera.h"
#include "Arduino.h"
#include <WiFi.h>
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems

// #define CAMERA_MODEL_AI_THINKER //ESP32-CAM
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

#define init_xclk_freq_hz      20000000
#define init_pixel_format      PIXFORMAT_JPEG
#define init_frame_size        FRAMESIZE_UXGA
#define init_jpeg_quality      20
#define init_fb_count          2

class photo
{
private:
    const int bauds=115200;
#if 0
    const char* ssid = "alterno.office"; ///  wifi ssid 
    const char* password = "sand.battery";
#endif
#if 1
    const char* ssid = "ecovi.iot"; ///  wifi ssid 
    const char* password = "sandbattery";
#endif

    const char* ntpServer = "pool.ntp.org"; // internet time server
    const long  gmtOffset_sec = 3600; // internet time server
    const int   daylightOffset_sec = 3600; // internet time server

public:
    photo();
    //static esp_err_t capture_handler(httpd_req_t *req);
    void startCameraServer();
    void setup();

};

extern photo _photo;
