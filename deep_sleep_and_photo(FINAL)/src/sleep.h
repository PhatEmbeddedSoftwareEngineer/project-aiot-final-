#include <Arduino.h>



class lightSleep
{
private:
    const int bauds=115200;
public:
    bool complete=false;
    lightSleep();
    void setTimeWakeup();
    void startSleep();
    void fcnAfterWakeup();
};

extern lightSleep _sleep;
