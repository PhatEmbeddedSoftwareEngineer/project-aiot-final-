#include <Arduino.h>

class flash
{
private:
    const int flashPin=4;
public:

    void setupFlashPin();

};

extern flash _flash;