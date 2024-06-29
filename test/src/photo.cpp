#include "photo.h"
#include "sleep.h"

WebServer server(80);

photo::photo()
{
    Serial.begin(115200);
}

void photo::refeshWeb()
{
    
}

void photo::serveJpg()
{
    auto frame = esp32cam::capture();
    if (frame == nullptr)
    {
        Serial.println("CAPTURE FAIL");
        server.send(503,"","");
        return;
    }
    Serial.printf("CAPTURE OK %dx%d %db\n", frame->getWidth(), frame->getHeight(),
                static_cast<int>(frame->size()));
    server.setContentLength(frame->size());
    //_photo.refeshWeb();
    server.send(200, "image/jpeg");
    WiFiClient client = server.client();
    frame->writeTo(client);
}
void photo::handleJpgLo()
{
    if (!esp32cam::Camera.changeResolution(loRes)) {
        Serial.println("SET-LO-RES FAIL");
    }
    _photo.serveJpg();
}

void photo::handleJpgHi()
{
    if (!esp32cam::Camera.changeResolution(hiRes)) {
        Serial.println("SET-HI-RES FAIL");
    }
    _photo.serveJpg();
}

void photo::handleJpgMid()
{
    if (!esp32cam::Camera.changeResolution(midRes)) {
        Serial.println("SET-MID-RES FAIL");
    }
    _photo.serveJpg();
}

void photo::setupJpg()
{
    Serial.println();
    {
        using namespace esp32cam;
        Config cfg;
        cfg.setPins(pins::AiThinker);
        cfg.setResolution(hiRes);
        cfg.setBufferCount(2);
        cfg.setJpeg(80);

        bool ok = Camera.begin(cfg);
        Serial.println(ok ? "CAMERA OK": "CAMERA FAIL");

    }
    
    // lat nguoc hinh anh
    sensor_t *s = esp_camera_sensor_get();
    s->set_vflip(s,1); // lat nguoc theo chieu doc (0: khong lat, 1: lat)
    s->set_hmirror(s,1); // lat nguoc theo chieu ngang (0: khong lat, 1:lat)
#if 1
    WiFi.persistent(false);
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid,pass);
    int cnt=0;
    while(WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.printf(">");
        cnt++;
        if(cnt==30)
            esp_restart();
    }
#endif
    Serial.print("http://");
    Serial.println(WiFi.localIP());
    
    Serial.println("  /photo-lo.jpg");
    Serial.println("  /photo.jpg");
    Serial.println("  /photo-mid.jpg");

    server.on("/photo-lo.jpg", handleJpgLo);
    server.on("/photo.jpg", handleJpgHi);
    server.on("/photo-mid.jpg", handleJpgMid);
    
    server.begin();
}

void photo::runJpg()
{
    server.handleClient();
    //_sleep.complete=true;
    
}


photo _photo;