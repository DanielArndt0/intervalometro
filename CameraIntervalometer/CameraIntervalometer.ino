#include "Intervalometer.h"

Intervalometer intervalometer;

void setup()
{
    intervalometer.begin();
}

void loop()
{
    intervalometer.update();
}
