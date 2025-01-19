#ifndef CAMERA_MANAGER_H
#define CAMERA_MANAGER_H

#include "config.h"
#include "esp_camera.h" // Asegúrate de incluir esta biblioteca si no está incluida

class CameraManager {
private:
    camera_config_t config;
    bool cameraInitialized = false;
    bool nightModeEnabled = false;
    uint8_t lastBrightness = 0;
    unsigned long lastLightCheck = 0;

public:
    CameraManager();
    bool begin();
    void setNightMode(bool enable);
    void setEffect(int effect);
    void adjustToLight();
    camera_fb_t* captureFrame();
    void returnFrame(camera_fb_t* fb);
    void setQuality(int quality);
    void setFrameSize(framesize_t size);
    void setContrast(int level);
    void setBrightness(int level);
    void setSaturation(int level);
    void setSpecialEffect(int effect);
    bool flashLight(bool on);

    // Nueva función: detectMotion
    bool detectMotion(camera_fb_t* current, camera_fb_t* previous);

private:
    uint8_t checkBrightness();
    void initCameraConfig();
    bool configureSensor();
};

#endif // CAMERA_MANAGER_H
