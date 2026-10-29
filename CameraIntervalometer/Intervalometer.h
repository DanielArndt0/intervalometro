#pragma once

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

class Intervalometer {
public:
    Intervalometer();

    void begin();
    void update();

private:
    void configure();
    void render(unsigned long now);
    void printTwoDigits(uint8_t value);

    Adafruit_SSD1306 display_;
    uint16_t intervalSeconds_;
    uint32_t photos_;
    unsigned long startedAt_;
    unsigned long lastShotAt_;
    unsigned long lastDisplayAt_;
    bool ready_;
};
