#include <Wire.h>
#include "Intervalometer.h"

namespace {
constexpr uint8_t POT_PIN = A0;
constexpr uint8_t BUTTON_PIN = 2;
constexpr uint8_t SHUTTER_PIN = 3;
constexpr uint16_t MAX_INTERVAL_SECONDS = 120;
constexpr unsigned long DISPLAY_MS = 500;
}

Intervalometer::Intervalometer()
    : display_(128, 64, &Wire, -1),
      intervalSeconds_(1),
      photos_(0),
      startedAt_(0),
      lastShotAt_(0),
      lastDisplayAt_(0),
      ready_(false)
{
}

void Intervalometer::begin()
{
    pinMode(BUTTON_PIN, INPUT);
    pinMode(SHUTTER_PIN, OUTPUT);
    digitalWrite(SHUTTER_PIN, LOW);

    Wire.begin();
    if (!display_.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        return;
    }

    display_.setTextColor(SSD1306_WHITE);
    configure();

    startedAt_ = millis();
    lastShotAt_ = startedAt_;
    lastDisplayAt_ = startedAt_;
    ready_ = true;
    render(startedAt_);
}

void Intervalometer::configure()
{
    uint16_t previous = 0;

    while (true) {
        const uint16_t selected = static_cast<uint16_t>(
            map(analogRead(POT_PIN), 0, 1023, 1, MAX_INTERVAL_SECONDS));

        if (selected != previous) {
            intervalSeconds_ = selected;
            previous = selected;

            display_.clearDisplay();
            display_.setTextSize(2);
            display_.setCursor(10, 10);
            display_.print(F("Periodo:"));
            display_.setCursor(10, 40);
            display_.print(intervalSeconds_);
            display_.print(F(" s"));
            display_.display();
        }

        if (digitalRead(BUTTON_PIN) == HIGH) {
            delay(30);
            if (digitalRead(BUTTON_PIN) == HIGH) {
                break;
            }
        }

        delay(20);
    }
}

void Intervalometer::update()
{
    if (!ready_) {
        return;
    }

    const unsigned long now = millis();

    if (now - lastShotAt_ >= static_cast<unsigned long>(intervalSeconds_) * 1000UL) {
        lastShotAt_ = now;
        digitalWrite(SHUTTER_PIN, HIGH);
        delay(2);
        digitalWrite(SHUTTER_PIN, LOW);
        ++photos_;
    }

    if (now - lastDisplayAt_ >= DISPLAY_MS) {
        lastDisplayAt_ = now;
        render(now);
    }
}

void Intervalometer::render(unsigned long now)
{
    const unsigned long elapsed = (now - startedAt_) / 1000UL;
    const uint8_t hours = (elapsed / 3600UL) % 100UL;
    const uint8_t minutes = (elapsed / 60UL) % 60UL;
    const uint8_t seconds = elapsed % 60UL;

    display_.clearDisplay();
    display_.setTextSize(2);
    display_.setCursor(15, 5);
    printTwoDigits(hours);
    display_.print(':');
    printTwoDigits(minutes);
    display_.print(':');
    printTwoDigits(seconds);

    display_.setTextSize(1);
    display_.setCursor(15, 30);
    display_.print(F("Intervalo: "));
    display_.print(intervalSeconds_);
    display_.print(F(" s"));

    display_.setTextSize(2);
    display_.setCursor(15, 45);
    display_.print(F("Pic:"));
    display_.print(photos_);
    display_.display();
}

void Intervalometer::printTwoDigits(uint8_t value)
{
    if (value < 10) {
        display_.print('0');
    }

    display_.print(value);
}
