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
    esp_light_sleep_start();

}

void lightSleep::fcnAfterWakeup()
{
    // Khi tỉnh dậy từ chế độ ngủ nhẹ, chương trình sẽ tiếp tục từ đây
    Serial.println("Awake now");

    // Serial.println("turn on");
    // delay(1000);
    // Serial.println("turn off");
    // delay(1000);
    int cnt=0;
    int flag=0;
    while(flag==0)
    {
        _photo.setupJpg();
        _photo.runJpg();
        _mqtt.runMqtt();
        cnt++;
        if(cnt==2)
        {
            flag++;
        }
            
        
    }
    
    
}


lightSleep _sleep;
