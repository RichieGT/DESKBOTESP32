#include "config.h"

// Credenciales WiFi
const char* ssid = "Familia DLO WRK";
const char* password = "Of1c1na123";

// NTP Server
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -18000;  // GMT-5
const int daylightOffset_sec = 0;

// Paletas de colores
const ColorPalette colorPalettes[] = {
    {0xFB9F, 0xFE7F, 0xF81F, 0x0000, 0xFFFF}, // Kawaii
    {0xFFFF, 0xC618, 0x8410, 0x0000, 0xFFFF}, // Minimal
    {0x07FF, 0xF81F, 0xFFE0, 0x0000, 0xFFFF}, // Neon
    {0x881F, 0x7BCF, 0xFFE0, 0x0000, 0xFFFF}, // Space
    {0xF7DE, 0xFBDE, 0xE71C, 0x0000, 0xFFFF}  // Korean
};

// Instancias globales
Adafruit_GC9A01A tft(TFT_CS, TFT_DC);
Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);