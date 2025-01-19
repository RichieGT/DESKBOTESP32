#include "NeoPixelManager.h"

NeoPixelManager::NeoPixelManager(Adafruit_NeoPixel& strip) : pixels(strip) {
    pixels.begin();
    pixels.setBrightness(50);
    pixels.show();
}

void NeoPixelManager::setColor(uint8_t r, uint8_t g, uint8_t b) {
    animationMode = 0;
    breathing = false;
    currentColor = pixels.Color(r, g, b);
    pixels.setPixelColor(0, currentColor);
    pixels.show();
}

void NeoPixelManager::setColorForExpression(RobotExpression expr) {
    switch (expr) {
        case EXPR_NORMAL:
            setColor(0, 255, 255);  // Cyan
            break;
        case EXPR_HAPPY:
            setColor(255, 255, 0);  // Amarillo
            startBreathing();
            break;
        case EXPR_SAD:
            setColor(0, 0, 255);    // Azul
            startBreathing();
            break;
        case EXPR_SURPRISED:
            setColor(255, 0, 255);  // Magenta
            break;
        case EXPR_SLEEPING:
            startBreathing(0, 0, 64);  // Azul suave
            break;
        default:
            setColor(0, 255, 0);    // Verde
    }
}

void NeoPixelManager::setColorForClockTheme(ClockTheme theme) {
    switch (theme) {
        case THEME_KAWAII:
            setColor(255, 192, 203);  // Rosa
            break;
        case THEME_MINIMAL:
            setColor(255, 255, 255);  // Blanco
            break;
        case THEME_NEON:
            startRainbow();
            break;
        case THEME_SPACE:
            startBreathing(64, 0, 128);  // Púrpura espacial
            break;
        case THEME_KOREAN:
            setColor(255, 182, 193);  // Rosa K-pop
            break;
    }
}

void NeoPixelManager::update() {
    unsigned long currentTime = millis();
    if (currentTime - lastUpdate < 20) return;

    handleAnimation();
    lastUpdate = currentTime;
}

void NeoPixelManager::handleAnimation() {
    switch (animationMode) {
        case 1:
            updateBreathing();
            break;
        case 2:
            updateRainbow();
            break;
    }
}

void NeoPixelManager::startBreathing(uint8_t r, uint8_t g, uint8_t b) {
    breathing = true;
    animationMode = 1;
    currentColor = pixels.Color(r, g, b);
    breathValue = 0;
    breathDirection = 1;
}

void NeoPixelManager::stopBreathing() {
    breathing = false;
    animationMode = 0;
    pixels.setBrightness(50);
    pixels.setPixelColor(0, currentColor);
    pixels.show();
}

void NeoPixelManager::startRainbow() {
    animationMode = 2;
    breathing = false;
}

void NeoPixelManager::setBrightness(uint8_t brightness) {
    pixels.setBrightness(brightness);
    pixels.show();
}

void NeoPixelManager::updateBreathing() {
    breathValue += breathDirection * 0.05;
    
    if (breathValue >= 1.0) {
        breathValue = 1.0;
        breathDirection = -1;
    } else if (breathValue <= 0.0) {
        breathValue = 0.0;
        breathDirection = 1;
    }
    
    uint8_t brightness = (uint8_t)(breathValue * 50);
    pixels.setBrightness(brightness);
    pixels.setPixelColor(0, currentColor);
    pixels.show();
}

void NeoPixelManager::updateRainbow() {
    unsigned long currentTime = millis();
    if (currentTime - lastColorChange >= 50) {  // Actualizar cada 50ms
        currentHue += 256;  // Avanzar en el ciclo de color
        pixels.setPixelColor(0, pixels.gamma32(pixels.ColorHSV(currentHue)));
        pixels.show();
        lastColorChange = currentTime;
    }
}

void NeoPixelManager::pulse(uint32_t color, int duration) {
    // Guardar el color y modo actuales
    uint32_t originalColor = currentColor;
    int originalMode = animationMode;
    
    // Efecto de pulso
    for (int i = 0; i <= 100; i++) {
        float brightness = sin((i * PI) / 100);
        pixels.setBrightness(brightness * 255);
        pixels.setPixelColor(0, color);
        pixels.show();
        delay(duration / 200);
    }
    
    // Restaurar estado original
    currentColor = originalColor;
    animationMode = originalMode;
    pixels.setBrightness(50);
    pixels.setPixelColor(0, currentColor);
    pixels.show();
}

void NeoPixelManager::flash(uint32_t color, int count, int duration) {
    uint32_t originalColor = currentColor;
    int originalMode = animationMode;
    
    for (int i = 0; i < count; i++) {
        pixels.setPixelColor(0, color);
        pixels.show();
        delay(duration / 2);
        
        pixels.setPixelColor(0, 0);  // Apagar
        pixels.show();
        delay(duration / 2);
    }
    
    currentColor = originalColor;
    animationMode = originalMode;
    pixels.setPixelColor(0, currentColor);
    pixels.show();
}