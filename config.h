#ifndef CONFIG_H
#define CONFIG_H

#include <WiFi.h>
#include <WebServer.h>
#include <BluetoothSerial.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_GC9A01A.h>
#include <Adafruit_NeoPixel.h>
#include "esp_camera.h"
#include <ArduinoJson.h>
#include "time.h"

// Definiciones de pines
#define TFT_CS 15
#define TFT_DC 2
#define TOUCH_PIN 14
#define NEOPIXEL_PIN 12
#define NUM_PIXELS 1

// Dimensiones de la pantalla
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 240
#define ROBOT_EYE_X_LEFT 80
#define ROBOT_EYE_X_RIGHT 160
#define ROBOT_EYE_Y 120

// Credenciales WiFi
extern const char* ssid;
extern const char* password;

// NTP Server
extern const char* ntpServer;
extern const long gmtOffset_sec;
extern const int daylightOffset_sec;

// Enumeraciones
enum DisplayMode {
    MODE_ROBOT,
    MODE_CLOCK,
    MODE_CAMERA
};

enum RobotExpression {
    EXPR_NORMAL,
    EXPR_LOOK_DOWN,
    EXPR_LOOK_UP,
    EXPR_LOOK_RIGHT,
    EXPR_LOOK_LEFT,
    EXPR_SURPRISED,
    EXPR_SLEEPING,
    EXPR_HAPPY,
    EXPR_SAD
};

enum ClockTheme {
    THEME_KAWAII,
    THEME_MINIMAL,
    THEME_NEON,
    THEME_SPACE,
    THEME_KOREAN
};

// Estructuras
struct TimeData {
    int hours;
    int minutes;
    int seconds;
    bool is24Hour;
};

struct ColorPalette {
    uint16_t primary;
    uint16_t secondary;
    uint16_t accent;
    uint16_t background;
    uint16_t text;
};

// Configuración de pines de la cámara ESP32-CAM
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

// Declaraciones externas
extern const ColorPalette colorPalettes[];
extern WebServer server;
extern DisplayMode currentMode;
extern Adafruit_GC9A01A tft;
extern Adafruit_NeoPixel pixels;

#endif // CONFIG_H