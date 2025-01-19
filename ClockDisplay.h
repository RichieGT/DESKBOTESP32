#ifndef CLOCK_DISPLAY_H
#define CLOCK_DISPLAY_H

#include "config.h"

class ClockDisplay {
private:
    Adafruit_GC9A01A& tft;
    ClockTheme currentTheme;
    TimeData currentTime;
    float animationProgress = 0;
    unsigned long lastUpdate = 0;
    
    struct DecoElement {
        int16_t x, y;
        float scale;
        float rotation;
        bool active;
        uint16_t color;
    };
    
    static const int MAX_DECO_ELEMENTS = 8;
    DecoElement decorations[MAX_DECO_ELEMENTS];

public:
    ClockDisplay(Adafruit_GC9A01A& display);
    void setTheme(ClockTheme theme);
    void updateTime(TimeData newTime);
    void update();

private:
    void initDecorations();
    void draw();
    void drawKawaiiClock();
    void drawPastelGradient();
    void drawKawaiiDecorations();
    void drawKawaiiNumbers();
    void drawKawaiiHands();
    void drawKawaiiHand(int centerX, int centerY, int length, float angle, int width, uint16_t color, bool rounded);
    void drawKawaiiFace();
    void drawKawaiiEye(int x, int y, uint16_t color);
    void drawKawaiiStar(int x, int y, uint16_t color);
    void drawKawaiiHeart(int x, int y, uint16_t color);
    
    void drawMinimalClock();
    void drawMinimalMarkers();
    void drawMinimalNumbers();
    void drawMinimalHands();
    void drawMinimalHand(int centerX, int centerY, int length, float angle, int width, uint16_t color);
    
    void drawNeonClock();
    void drawNeonGlow();
    void drawNeonDigits();
    void drawNeonHands();
    void drawNeonHand(int centerX, int centerY, int length, float angle, uint16_t color);
    
    void drawSpaceClock();
    void drawStarfield();
    void drawPlanetClock();
    void drawPlanetRing(int centerX, int centerY, int radius, int width);
    void drawSpaceHands();
    void drawSpaceHand(int centerX, int centerY, int length, float angle, int width, uint16_t color);
    void drawOrbits();
    
    void drawKoreanClock();
    void drawKoreanPattern();
    void drawHangulNumbers();
    void drawKoreanHands();
    void drawBrushHand(int centerX, int centerY, int length, float angle, uint16_t color);
    void drawKoreanDecorations();
    void drawSoundWave();
    
    void updateAnimations();
    uint16_t blendColors(uint16_t color1, uint16_t color2, float ratio);
    uint16_t fadeColor(uint16_t color, float factor);
    void animateThemeTransition(ClockTheme newTheme);
};

#endif // CLOCK_DISPLAY_H