#include "sleep.h"
#include "mqtt.h"
#include "photo.h"

lightSleep::lightSleep()
{
    Serial.begin(bauds);
}

void lightSleep::setTimeWakeup()
{
    Serial.println();
    // Thiết lập timer tỉnh dậy sau 10 giây
    esp_sleep_enable_timer_wakeup(10 * 1000000); // 10 seconds
}
void lightSleep::startSleep()
{
    // In ra "Entering deep sleep" và bắt đầu chế độ ngủ nhẹ
    Serial.println("Start Sleep");
    //esp_light_sleep_start();
    esp_deep_sleep_start();

}

void lightSleep::fcnAfterWakeup()
{
    // Khi tỉnh dậy từ chế độ ngủ nhẹ, chương trình sẽ tiếp tục từ đây
    Serial.println("Awake now");
    // for(int i=0;i<20;i++)
    // {
    //     //_photo.setup();
    //     _mqtt.runMqtt();
        
    // }
    // delay(2000);
    
}


lightSleep _sleep;
