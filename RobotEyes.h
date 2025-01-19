#ifndef ROBOT_EYES_H
#define ROBOT_EYES_H

#include "config.h"

class RobotEyes {
private:
    Adafruit_GC9A01A& tft;
    RobotExpression currentExpression;
    uint16_t mainColor;
    uint16_t glowColor;
    const int baseEyeWidth = 50;
    const int baseEyeHeight = 50;
    const int roundness = 8;
    float emotionIntensity = 1.0;
    
    // Variables para micro-movimientos
    float microMovementX = 0;
    float microMovementY = 0;
    unsigned long lastMicroMove = 0;
    const int MICRO_MOVE_INTERVAL = 2000;
    
    // Variables para efectos de rebote
    float bounceOffset = 0;
    bool isBouncing = false;
    
    // Control de parpadeo
    unsigned long lastBlink = 0;
    int blinkInterval = 3000;
    bool isBlinking = false;
    
    struct EyePosition {
        int x, y;
        float scale;
        int offsetX, offsetY;
        float squeezeX, squeezeY;
    } leftEye, rightEye;

public:
    RobotEyes(Adafruit_GC9A01A& display);
    void setExpression(RobotExpression expr, bool withAnimation = true);
    void update();
    void setTheme(int theme);
    void drawEyes();
    void blink();
    RobotExpression getRandomExpression();

private:
    void updateMicroMovements();
    void updateBounceEffect();
    void updateEyePositions();
    void drawEye(int x, int y, int width, int height, int offsetX, int offsetY, uint16_t color);
    void drawExpressionDetails();
    void drawSparkles();
    void drawExclamationMarks();
    void drawExclamationMark(int x, int y);
    void drawZzz();
    void drawTears();
    void drawTear(int x, int y);
    void animateTransition(RobotExpression newExpr);
    uint16_t fadeColor(uint16_t color, int step);
};

#endif // ROBOT_EYES_H