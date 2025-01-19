#include "CameraManager.h"

// Constructor: inicializa la configuración de la cámara
CameraManager::CameraManager() {
    initCameraConfig();
}

// Configura los parámetros iniciales de la cámara
void CameraManager::initCameraConfig() {
    config.ledc_channel = LEDC_CHANNEL_0;
    config.ledc_timer = LEDC_TIMER_0;
    config.pin_d0 = Y2_GPIO_NUM;
    config.pin_d1 = Y3_GPIO_NUM;
    config.pin_d2 = Y4_GPIO_NUM;
    config.pin_d3 = Y5_GPIO_NUM;
    config.pin_d4 = Y6_GPIO_NUM;
    config.pin_d5 = Y7_GPIO_NUM;
    config.pin_d6 = Y8_GPIO_NUM;
    config.pin_d7 = Y9_GPIO_NUM;
    config.pin_xclk = XCLK_GPIO_NUM;
    config.pin_pclk = PCLK_GPIO_NUM;
    config.pin_vsync = VSYNC_GPIO_NUM;
    config.pin_href = HREF_GPIO_NUM;
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_JPEG;
    config.frame_size = FRAMESIZE_QVGA;
    config.jpeg_quality = 10;
    config.fb_count = 1;
}

// Inicializa la cámara
bool CameraManager::begin() {
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x\n", err);
        return false;
    }
    cameraInitialized = true;
    return true;
}

// Captura un frame
camera_fb_t* CameraManager::captureFrame() {
    if (!cameraInitialized) {
        Serial.println("Camera not initialized!");
        return nullptr;
    }
    return esp_camera_fb_get();
}

// Devuelve un frame al buffer
void CameraManager::returnFrame(camera_fb_t* fb) {
    if (fb) {
        esp_camera_fb_return(fb);
    }
}

// Detecta movimiento entre dos frames
bool CameraManager::detectMotion(camera_fb_t* current, camera_fb_t* previous) {
    if (!current || !previous) return false;

    int threshold = 30;
    int changedPixels = 0;

    uint8_t* curr_buf = current->buf;
    uint8_t* prev_buf = previous->buf;

    for (size_t i = 0; i < current->len; i += 4) {
        if (abs(curr_buf[i] - prev_buf[i]) > threshold) {
            changedPixels++;
        }
    }

    return (changedPixels > (current->len / 40));  // Cambios > 2.5%
}
