#include "sleep.h"
RTC_DATA_ATTR int bootCount = 0;

DeepSleep::DeepSleep()
{
    Serial.begin(baud);
}

void DeepSleep::print_wakeup_reason()
{
    esp_sleep_wakeup_cause_t wakeup_reason;

    wakeup_reason = esp_sleep_get_wakeup_cause();

    switch(wakeup_reason)
    {
        case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
        case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
        case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
        case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
        case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
        default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;

    }   
}

void DeepSleep::setupDeepSleep()
{
    ++bootCount;
    Serial.println("Boot number: " + String(bootCount));
    
    // print the wakeup reason
    _sleep.print_wakeup_reason();

    // configure wake up every 5s
    esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * us_TO_S_FACTOR);
    Serial.println("Setup ESP32 to sleep for every " + String(TIME_TO_SLEEP) +" Seconds");
    
}

void DeepSleep::runDeepSleep()
{
    Serial.println("going to sleep");
    Serial.flush(); 
    esp_deep_sleep_start();
    Serial.println("This will never be printed");

}

DeepSleep _sleep;
