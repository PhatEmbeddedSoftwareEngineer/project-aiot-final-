#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>
#include <esp32cam.h>
#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "soc/soc.h" //disable brownout problems
#include "soc/rtc_cntl_reg.h"  //disable brownout problems
#include "esp_http_server.h"

extern WebServer server;

static auto loRes = esp32cam::Resolution::find(320, 240);
static auto midRes = esp32cam::Resolution::find(350, 530);
static auto hiRes = esp32cam::Resolution::find(800, 600);

class photo
{
private:
    const int baud=115200;
    
    const char* ssid = "ecovi.iot";
    const char* pass = "sandbattery";

public:
    photo();
    void serveJpg();
    static void handleJpgLo();
    static void handleJpgHi();
    static void handleJpgMid();

    void setupJpg();

    void runJpg();

    void refeshWeb();

};

extern photo _photo;
