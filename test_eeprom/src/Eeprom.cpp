#include "Eeprom.h"

eeprom::eeprom()
{
    Serial.begin(bauds);
}

eeprom _eeprom;