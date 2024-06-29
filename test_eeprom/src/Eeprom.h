#include <Arduino.h>


class eeprom
{
private:
    const int bauds=115200;
public:
    eeprom();
};

extern eeprom _eeprom;
