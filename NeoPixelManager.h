#ifndef NEOPIXEL_MANAGER_H
#define NEOPIXEL_MANAGER_H

#include "config.h"

class NeoPixelManager {
private:
    Adafruit_NeoPixel& pixels;
    uint32_t currentColor = 0;
    bool breathing = false;
    float breathValue = 0;
    float breathDirection = 1;
    unsigned long lastUpdate = 0;
    int animationMode = 0; // 0: normal, 1: breathing, 2: rainbow
    unsigned long lastColorChange = 0;
    uint16_t currentHue = 0;

public:
    NeoPixelManager(Adafruit_NeoPixel& strip);
    
    void setColor(uint8_t r, uint8_t g, uint8_t b);
    void setColorForExpression(RobotExpression expr);
    void setColorForClockTheme(ClockTheme theme);
    void update();
    void startBreathing(uint8_t r = 255, uint8_t g = 255, uint8_t b = 255);
    void stopBreathing();
    void startRainbow();
    void setBrightness(uint8_t brightness);
    void pulse(uint32_t color, int duration = 500);
    void flash(uint32_t color, int count = 1, int duration = 200);
    
private:
    void updateBreathing();
    void updateRainbow();
    void handleAnimation();
};

#endif // NEOPIXEL_MANAGER_H