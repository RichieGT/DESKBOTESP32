#include "ClockDisplay.h"

ClockDisplay::ClockDisplay(Adafruit_GC9A01A& display) 
    : tft(display), currentTheme(THEME_KAWAII) {
    initDecorations();
}

void ClockDisplay::setTheme(ClockTheme theme) {
    if (currentTheme != theme) {
        animateThemeTransition(theme);
        currentTheme = theme;
    }
}

void ClockDisplay::updateTime(TimeData newTime) {
    if (newTime.seconds != currentTime.seconds) {
        currentTime = newTime;
        draw();
    }
}

void ClockDisplay::update() {
    unsigned long currentMillis = millis();
    if (currentMillis - lastUpdate >= 50) {
        updateAnimations();
        lastUpdate = currentMillis;
    }
}

void ClockDisplay::initDecorations() {
    for (int i = 0; i < MAX_DECO_ELEMENTS; i++) {
        decorations[i].x = random(SCREEN_WIDTH);
        decorations[i].y = random(SCREEN_HEIGHT);
        decorations[i].scale = 0.5 + random(100) / 100.0;
        decorations[i].rotation = random(360) * PI / 180;
        decorations[i].active = true;
        decorations[i].color = colorPalettes[currentTheme].accent;
    }
}

void ClockDisplay::draw() {
    tft.fillScreen(colorPalettes[currentTheme].background);
    
    switch (currentTheme) {
        case THEME_KAWAII:
            drawKawaiiClock();
            break;
        case THEME_MINIMAL:
            drawMinimalClock();
            break;
        case THEME_NEON:
            drawNeonClock();
            break;
        case THEME_SPACE:
            drawSpaceClock();
            break;
        case THEME_KOREAN:
            drawKoreanClock();
            break;
    }
}

void ClockDisplay::drawKawaiiClock() {
    drawPastelGradient();
    drawKawaiiDecorations();
    drawKawaiiNumbers();
    drawKawaiiHands();
    drawKawaiiFace();
}

void ClockDisplay::drawPastelGradient() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        float ratio = (float)i / SCREEN_HEIGHT;
        uint16_t color = blendColors(
            colorPalettes[THEME_KAWAII].primary,
            colorPalettes[THEME_KAWAII].secondary,
            ratio
        );
        tft.drawFastHLine(0, i, SCREEN_WIDTH, color);
    }
}

void ClockDisplay::drawKawaiiDecorations() {
    static const uint8_t NUM_STARS = 6;
    for (int i = 0; i < NUM_STARS; i++) {
        int x = SCREEN_WIDTH/2 + cos(2*PI*i/NUM_STARS) * 100;
        int y = SCREEN_HEIGHT/2 + sin(2*PI*i/NUM_STARS) * 100;
        drawKawaiiStar(x, y, colorPalettes[THEME_KAWAII].accent);
    }

    // Dibujar corazones decorativos
    for (int i = 0; i < 4; i++) {
        int x = SCREEN_WIDTH/2 + cos(2*PI*i/4) * 80;
        int y = SCREEN_HEIGHT/2 + sin(2*PI*i/4) * 80;
        drawKawaiiHeart(x, y, colorPalettes[THEME_KAWAII].accent);
    }
}

void ClockDisplay::drawKawaiiNumbers() {
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    int radius = 90;

    tft.setTextSize(2);
    for (int i = 1; i <= 12; i++) {
        float angle = (i - 3) * 30 * PI / 180;
        int x = centerX + cos(angle) * radius;
        int y = centerY + sin(angle) * radius;

        if (i < 10) x -= 6;
        else x -= 12;
        y -= 8;

        tft.fillCircle(x + 6, y + 8, 15, fadeColor(colorPalettes[THEME_KAWAII].secondary, 0.7));
        
        tft.setTextColor(colorPalettes[THEME_KAWAII].text);
        tft.setCursor(x, y);
        tft.print(i);

        if (i == 3 || i == 6 || i == 9 || i == 12) {
            drawKawaiiHeart(x + 20, y, colorPalettes[THEME_KAWAII].accent);
        }
    }
}

void ClockDisplay::drawKawaiiHand(int centerX, int centerY, int length, float angle, 
                                int width, uint16_t color, bool rounded) {
    int endX = centerX + cos(angle) * length;
    int endY = centerY + sin(angle) * length;
    
    // Dibujar sombra
    tft.drawLine(centerX+1, centerY+1, endX+1, endY+1, fadeColor(color, 0.5));
    
    // Dibujar manecilla principal
    for (int i = -width/2; i <= width/2; i++) {
        int offsetX = -sin(angle) * i;
        int offsetY = cos(angle) * i;
        tft.drawLine(centerX + offsetX, centerY + offsetY, 
                    endX + offsetX, endY + offsetY, color);
    }

    if (rounded) {
        tft.fillCircle(endX, endY, width/2 + 1, color);
    }
}

void ClockDisplay::drawKawaiiFace() {
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    uint16_t color = colorPalettes[THEME_KAWAII].accent;

    // Ojos
    int eyeSpacing = 30;
    drawKawaiiEye(centerX - eyeSpacing, centerY - 40, color);
    drawKawaiiEye(centerX + eyeSpacing, centerY - 40, color);

    // Mejillas sonrojadas
    tft.fillCircle(centerX - 40, centerY - 35, 8, fadeColor(0xF800, 0.3));
    tft.fillCircle(centerX + 40, centerY - 35, 8, fadeColor(0xF800, 0.3));

    // Sonrisa
    for (int i = 0; i < 3; i++) {
        float startAngle = PI/4; // 45 grados
        float endAngle = PI*3/4; // 135 grados
        float radius = 20 + i;
        
        for (float angle = startAngle; angle <= endAngle; angle += 0.1) {
            int x = centerX + cos(angle) * radius;
            int y = (centerY - 30) + sin(angle) * radius;
            tft.drawPixel(x, y, color);
        }
    }
}

void ClockDisplay::drawMinimalClock() {
    drawMinimalMarkers();
    drawMinimalNumbers();
    drawMinimalHands();
}

void ClockDisplay::drawMinimalMarkers() {
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    int radius = 100;

    for (int i = 0; i < 60; i++) {
        float angle = i * 6 * PI / 180;
        int len = (i % 5 == 0) ? 15 : 5;
        int outerX = centerX + cos(angle) * radius;
        int outerY = centerY + sin(angle) * radius;
        int innerX = centerX + cos(angle) * (radius - len);
        int innerY = centerY + sin(angle) * (radius - len);
        
        uint16_t color = (i % 5 == 0) ? 
            colorPalettes[THEME_MINIMAL].primary : 
            colorPalettes[THEME_MINIMAL].secondary;
        
        tft.drawLine(innerX, innerY, outerX, outerY, color);
    }
}

void ClockDisplay::drawNeonClock() {
    drawNeonGlow();
    drawNeonDigits();
    drawNeonHands();
}

void ClockDisplay::drawNeonGlow() {
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    
    for (int r = 110; r > 90; r--) {
        float alpha = (110 - r) / 20.0;
        uint16_t color = fadeColor(colorPalettes[THEME_NEON].primary, alpha);
        tft.drawCircle(centerX, centerY, r, color);
    }
}

void ClockDisplay::drawSpaceClock() {
    drawStarfield();
    drawPlanetClock();
    drawSpaceHands();
    drawOrbits();
}

void ClockDisplay::drawStarfield() {
    for (int i = 0; i < 100; i++) {
        int x = random(SCREEN_WIDTH);
        int y = random(SCREEN_HEIGHT);
        int brightness = random(4);
        uint16_t color = fadeColor(colorPalettes[THEME_SPACE].accent, brightness/3.0);
        
        if (random(3) == 0) {
            tft.drawPixel(x-1, y, color);
            tft.drawPixel(x+1, y, color);
            tft.drawPixel(x, y-1, color);
            tft.drawPixel(x, y+1, color);
        } else {
            tft.drawPixel(x, y, color);
        }
    }
}

void ClockDisplay::drawKoreanClock() {
    drawKoreanPattern();
    drawHangulNumbers();
    drawKoreanHands();
    drawKoreanDecorations();
}

void ClockDisplay::drawKoreanPattern() {
    for (int x = 0; x < SCREEN_WIDTH; x += 20) {
        for (int y = 0; y < SCREEN_HEIGHT; y += 20) {
            uint16_t color = fadeColor(colorPalettes[THEME_KOREAN].secondary, (x + y) % 4);
            tft.drawRoundRect(x, y, 20, 20, 5, color);
        }
    }
}

void ClockDisplay::drawHangulNumbers() {
    const char* hangulHours[] = {
        "십이", "일", "이", "삼", "사", "오",
        "육", "칠", "팔", "구", "십", "십일"
    };
    
    int centerX = SCREEN_WIDTH / 2;
    int centerY = SCREEN_HEIGHT / 2;
    int radius = 80;
    
    tft.setTextSize(2);
    
    for (int i = 0; i < 12; i++) {
        float angle = (i - 2) * 30 * PI / 180;
        int x = centerX + cos(angle) * radius;
        int y = centerY + sin(angle) * radius;
        
        x -= strlen(hangulHours[i]) * 6;
        y -= 8;
        
        tft.setTextColor(fadeColor(colorPalettes[THEME_KOREAN].primary, 0.7));
        tft.setCursor(x+1, y+1);
        tft.print(hangulHours[i]);
        
        tft.setTextColor(colorPalettes[THEME_KOREAN].primary);
        tft.setCursor(x, y);
        tft.print(hangulHours[i]);
    }
}

void ClockDisplay::updateAnimations() {
    for (int i = 0; i < MAX_DECO_ELEMENTS; i++) {
        if (decorations[i].active) {
            decorations[i].rotation += 0.05;
            decorations[i].y += sin(decorations[i].rotation) * 0.5;
            
            if (decorations[i].y > SCREEN_HEIGHT) {
                decorations[i].y = -20;
                decorations[i].x = random(SCREEN_WIDTH);
            }
        }
    }
}

uint16_t ClockDisplay::blendColors(uint16_t color1, uint16_t color2, float ratio) {
    uint8_t r1 = (color1 >> 11) & 0x1F;
    uint8_t g1 = (color1 >> 5) & 0x3F;
    uint8_t b1 = color1 & 0x1F;
    
    uint8_t r2 = (color2 >> 11) & 0x1F;
    uint8_t g2 = (color2 >> 5) & 0x3F;
    uint8_t b2 = color2 & 0x1F;
    
    uint8_t r = r1 + (r2 - r1) * ratio;
    uint8_t g = g1 + (g2 - g1) * ratio;
    uint8_t b = b1 + (b2 - b1) * ratio;
    
    return (r << 11) | (g << 5) | b;
}

uint16_t ClockDisplay::fadeColor(uint16_t color, float factor) {
    uint8_t r = (color >> 11) & 0x1F;
    uint8_t g = (color >> 5) & 0x3F;
    uint8_t b = color & 0x1F;
    
    r = r * factor;
    g = g * factor;
    b = b * factor;
    
    return (r << 11) | (g << 5) | b;
}

void ClockDisplay::animateThemeTransition(ClockTheme newTheme) {
    for (int radius = 0; radius <= SCREEN_WIDTH + 20; radius += 8) {
        tft.fillCircle(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, radius, 
                      colorPalettes[newTheme].background);
        delay(5);
    }
}