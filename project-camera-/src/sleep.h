#include <Arduino.h>

#define us_TO_S_FACTOR 1000000
#define TIME_TO_SLEEP 15

extern RTC_DATA_ATTR int bootCount;



class DeepSleep
{
private:
    const int baud=115200;
public:
    
    DeepSleep();
    void print_wakeup_reason();

    void setupDeepSleep();
    

//void runDeepSleep();
    void runDeepSleep();

};

extern DeepSleep _sleep;