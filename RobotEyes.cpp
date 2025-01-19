#include "RobotEyes.h"

RobotEyes::RobotEyes(Adafruit_GC9A01A& display) 
    : tft(display), currentExpression(EXPR_NORMAL), mainColor(colorPalettes[0].primary) {
    leftEye = {ROBOT_EYE_X_LEFT, ROBOT_EYE_Y, 1.0, 0, 0, 1.0, 1.0};
    rightEye = {ROBOT_EYE_X_RIGHT, ROBOT_EYE_Y, 1.0, 0, 0, 1.0, 1.0};
    glowColor = mainColor;
}

void RobotEyes::setExpression(RobotExpression expr, bool withAnimation) {
    if (currentExpression == expr) return;
    
    if (withAnimation) {
        animateTransition(expr);
    } else {
        currentExpression = expr;
        updateEyePositions();
        drawEyes();
    }
}

void RobotEyes::update() {
    unsigned long currentTime = millis();
    
    if (currentTime - lastMicroMove > MICRO_MOVE_INTERVAL) {
        updateMicroMovements();
        lastMicroMove = currentTime;
    }
    
    if (isBouncing) {
        updateBounceEffect();
    }
}

void RobotEyes::setTheme(int theme) {
    if (theme >= 0 && theme < 6) {
        mainColor = colorPalettes[theme].primary;
        glowColor = mainColor;
        drawEyes();
    }
}

void RobotEyes::drawEyes() {
    tft.fillScreen(GC9A01A_BLACK);
    
    int finalOffsetX = microMovementX + bounceOffset;
    int finalOffsetY = microMovementY + bounceOffset;

    for (int i = 3; i >= 0; i--) {
        uint16_t currentGlow = fadeColor(glowColor, i);
        int glowSize = i * 2;
        
        drawEye(leftEye.x + finalOffsetX, leftEye.y + finalOffsetY, 
               baseEyeWidth * leftEye.scale + glowSize, 
               baseEyeHeight * leftEye.scale + glowSize,
               leftEye.offsetX, leftEye.offsetY, 
               currentGlow);

        drawEye(rightEye.x + finalOffsetX, rightEye.y + finalOffsetY,
               baseEyeWidth * rightEye.scale + glowSize,
               baseEyeHeight * rightEye.scale + glowSize,
               rightEye.offsetX, rightEye.offsetY,
               currentGlow);
    }

    drawExpressionDetails();
}

void RobotEyes::blink() {
    if (isBlinking) return;
    
    isBlinking = true;
    const int steps = 10;
    float originalScale = leftEye.scale;
    
    for (int i = steps; i >= 0; i--) {
        float scale = originalScale * i / steps;
        leftEye.scale = scale;
        rightEye.scale = scale;
        drawEyes();
        delay(20);
    }
    
    for (int i = 0; i <= steps; i++) {
        float scale = originalScale * i / steps;
        leftEye.scale = scale;
        rightEye.scale = scale;
        drawEyes();
        delay(20);
    }
    
    leftEye.scale = originalScale;
    rightEye.scale = originalScale;
    drawEyes();
    isBlinking = false;
}

RobotExpression RobotEyes::getRandomExpression() {
    return static_cast<RobotExpression>(random(EXPR_NORMAL, EXPR_SAD + 1));
}

void RobotEyes::updateMicroMovements() {
    if (currentExpression == EXPR_NORMAL) {
        microMovementX = (random(-10, 11) / 10.0) * 2;
        microMovementY = (random(-10, 11) / 10.0) * 2;
        drawEyes();
    }
}

void RobotEyes::updateBounceEffect() {
    static float bouncePhase = 0;
    bouncePhase += 0.2;
    bounceOffset = sin(bouncePhase) * 5;
    
    if (bouncePhase >= PI) {
        bouncePhase = 0;
        isBouncing = false;
        bounceOffset = 0;
    }
    drawEyes();
}

void RobotEyes::updateEyePositions() {
    leftEye.scale = rightEye.scale = 1.0;
    leftEye.offsetX = rightEye.offsetX = 0;
    leftEye.offsetY = rightEye.offsetY = 0;
    leftEye.squeezeX = rightEye.squeezeX = 1.0;
    leftEye.squeezeY = rightEye.squeezeY = 1.0;

    switch (currentExpression) {
        case EXPR_LOOK_DOWN:
            leftEye.offsetY = rightEye.offsetY = 25;
            leftEye.scale = rightEye.scale = 0.8;
            break;
        case EXPR_LOOK_UP:
            leftEye.offsetY = rightEye.offsetY = -25;
            leftEye.scale = rightEye.scale = 0.8;
            break;
        case EXPR_LOOK_RIGHT:
            leftEye.offsetX = rightEye.offsetX = 25;
            leftEye.scale = rightEye.scale = 0.8;
            break;
        case EXPR_LOOK_LEFT:
            leftEye.offsetX = rightEye.offsetX = -25;
            leftEye.scale = rightEye.scale = 0.8;
            break;
        case EXPR_SURPRISED:
            leftEye.scale = rightEye.scale = 1.5;
            break;
        case EXPR_SLEEPING:
            leftEye.scale = rightEye.scale = 0.1;
            break;
        case EXPR_HAPPY:
            leftEye.offsetY = rightEye.offsetY = -10;
            leftEye.scale = rightEye.scale = 0.6;
            leftEye.squeezeY = rightEye.squeezeY = 0.8;
            break;
        case EXPR_SAD:
            leftEye.offsetY = rightEye.offsetY = 10;
            leftEye.scale = rightEye.scale = 0.6;
            leftEye.squeezeY = rightEye.squeezeY = 1.2;
            break;
    }
}

void RobotEyes::drawEye(int x, int y, int width, int height, int offsetX, int offsetY, uint16_t color) {
    tft.fillRoundRect(x - width/2 + offsetX, 
                     y - height/2 + offsetY,
                     width, height, roundness, color);
}

void RobotEyes::drawExpressionDetails() {
    switch (currentExpression) {
        case EXPR_HAPPY:
            drawSparkles();
            break;
        case EXPR_SURPRISED:
            drawExclamationMarks();
            break;
        case EXPR_SLEEPING:
            drawZzz();
            break;
        case EXPR_SAD:
            drawTears();
            break;
    }
}

void RobotEyes::drawSparkles() {
    const int numSparkles = 6;
    for (int i = 0; i < numSparkles; i++) {
        int x = random(0, SCREEN_WIDTH);
        int y = random(0, SCREEN_HEIGHT);
        for (int j = 0; j < 5; j++) {
            float angle = j * 2 * PI / 5;
            int x1 = x + cos(angle) * 3;
            int y1 = y + sin(angle) * 3;
            tft.drawPixel(x1, y1, mainColor);
        }
    }
}

void RobotEyes::drawExclamationMarks() {
    drawExclamationMark(leftEye.x, leftEye.y - 40);
    drawExclamationMark(rightEye.x, rightEye.y - 40);
}

void RobotEyes::drawExclamationMark(int x, int y) {
    tft.fillRect(x - 2, y - 15, 4, 20, mainColor);
    tft.fillCircle(x, y + 10, 2, mainColor);
}

void RobotEyes::drawZzz() {
    int x = rightEye.x + 30;
    int y = rightEye.y - 30;
    
    for (int i = 0; i < 3; i++) {
        tft.drawChar(x + (i * 10), y + (i * 5), 'z', mainColor, GC9A01A_BLACK, 2);
    }
}

void RobotEyes::drawTears() {
    drawTear(leftEye.x, leftEye.y + 30);
    drawTear(rightEye.x, rightEye.y + 30);
}

void RobotEyes::drawTear(int x, int y) {
    tft.fillCircle(x, y, 3, 0x001F); // Azul
    tft.fillTriangle(x - 3, y, x + 3, y, x, y - 6, 0x001F);
}

void RobotEyes::animateTransition(RobotExpression newExpr) {
    const int steps = 10;
    EyePosition oldLeft = leftEye;
    EyePosition oldRight = rightEye;
    
    RobotExpression oldExpr = currentExpression;
    currentExpression = newExpr;
    updateEyePositions();
    EyePosition targetLeft = leftEye;
    EyePosition targetRight = rightEye;
    
    for (int i = 0; i <= steps; i++) {
        float t = (float)i / steps;
        
        leftEye.offsetX = oldLeft.offsetX + (targetLeft.offsetX - oldLeft.offsetX) * t;
        leftEye.offsetY = oldLeft.offsetY + (targetLeft.offsetY - oldLeft.offsetY) * t;
        leftEye.scale = oldLeft.scale + (targetLeft.scale - oldLeft.scale) * t;
        
        rightEye.offsetX = oldRight.offsetX + (targetRight.offsetX - oldRight.offsetX) * t;
        rightEye.offsetY = oldRight.offsetY + (targetRight.offsetY - oldRight.offsetY) * t;
        rightEye.scale = oldRight.scale + (targetRight.scale - oldRight.scale) * t;
        
        drawEyes();
        delay(20);
    }
}

uint16_t RobotEyes::fadeColor(uint16_t color, int step) {
    uint8_t r = (color >> 11) & 0x1F;
    uint8_t g = (color >> 5) & 0x3F;
    uint8_t b = color & 0x1F;
    
    float factor = (4 - step) / 4.0;
    r = r * factor;
    g = g * factor;
    b = b * factor;
    
    return (r << 11) | (g << 5) | b;
}