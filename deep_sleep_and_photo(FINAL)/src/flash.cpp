#include "flash.h"

void flash::setupFlashPin()
{
    pinMode(flashPin,OUTPUT);
    digitalWrite(flashPin,1);
    delay(500);
    digitalWrite(flashPin,0);
    delay(500);
}

flash _flash;
