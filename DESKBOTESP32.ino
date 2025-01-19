#include "config.h"
#include "RobotEyes.h"
#include "ClockDisplay.h"
#include "CameraManager.h"
#include "NeoPixelManager.h"

// Instancias globales
RobotEyes* robotEyes = nullptr;
ClockDisplay* clockDisplay = nullptr;
CameraManager* camera = nullptr;
NeoPixelManager* neopixelManager = nullptr;
WebServer server(80);
DisplayMode currentMode = MODE_ROBOT;

// Variables de control
unsigned long lastBlink = 0;
unsigned long lastModeCheck = 0;
unsigned long lastTimeUpdate = 0;
bool touchActive = false;
int touchSequenceStep = 0;
unsigned long touchStartTime = 0;

void handleTouchSensor(unsigned long currentMillis) {
    bool isTouched = digitalRead(TOUCH_PIN);
    
    // Inicio del toque
    if (isTouched && !touchActive) {
        touchActive = true;
        touchStartTime = currentMillis;
        touchSequenceStep = 0;
    }
    
    // Durante el toque
    if (touchActive && isTouched) {
        unsigned long touchDuration = currentMillis - touchStartTime;
        
        if (touchDuration >= touchSequenceStep * 1000) {
            switch(touchSequenceStep) {
                case 0: // Primera expresión: Sorprendido
                    robotEyes->setExpression(EXPR_SURPRISED);
                    neopixelManager->setColorForExpression(EXPR_SURPRISED);
                    break;
                case 1: // Segunda expresión: Feliz
                    robotEyes->setExpression(EXPR_HAPPY);
                    neopixelManager->setColorForExpression(EXPR_HAPPY);
                    break;
                case 2: // Tercera expresión: Aleatoria
                    RobotExpression randomExpr = robotEyes->getRandomExpression();
                    robotEyes->setExpression(randomExpr);
                    neopixelManager->setColorForExpression(randomExpr);
                    break;
            }
            if (touchSequenceStep < 3) touchSequenceStep++;
        }
    }
    
    // Fin del toque
    if (!isTouched && touchActive) {
        touchActive = false;
        robotEyes->setExpression(EXPR_NORMAL);
        neopixelManager->setColorForExpression(EXPR_NORMAL);
    }
}

void updateRobotMode(unsigned long currentMillis) {
    static unsigned long lastNormalModeStart = currentMillis;
    static bool inNormalSequence = true;
    
    // Actualizar animaciones de ojos
    robotEyes->update();
    
    // Manejar secuencia de modo normal
    if (inNormalSequence) {
        unsigned long normalModeDuration = currentMillis - lastNormalModeStart;
        
        // Parpadear cada 5 segundos
        if (currentMillis - lastBlink >= 5000) {
            robotEyes->blink();
            lastBlink = currentMillis;
        }
        
        // Expresión aleatoria después de 10 segundos
        if (normalModeDuration >= 10000 && normalModeDuration < 11000) {
            RobotExpression randomExpr = robotEyes->getRandomExpression();
            robotEyes->setExpression(randomExpr);
            neopixelManager->setColorForExpression(randomExpr);
            inNormalSequence = false;
        }
    } else {
        // Volver a normal después de 1 segundo
        if (currentMillis - lastNormalModeStart >= 11000) {
            robotEyes->setExpression(EXPR_NORMAL);
            neopixelManager->setColorForExpression(EXPR_NORMAL);
            lastNormalModeStart = currentMillis;
            inNormalSequence = true;
        }
    }
}

void updateClockMode(unsigned long currentMillis) {
    // Actualizar hora cada segundo
    if (currentMillis - lastTimeUpdate >= 1000) {
        struct tm timeinfo;
        if (getLocalTime(&timeinfo)) {
            TimeData newTime;
            newTime.hours = timeinfo.tm_hour;
            newTime.minutes = timeinfo.tm_min;
            newTime.seconds = timeinfo.tm_sec;
            newTime.is24Hour = true;
            
            clockDisplay->updateTime(newTime);
        }
        lastTimeUpdate = currentMillis;
    }
    
    // Actualizar animaciones del reloj
    clockDisplay->update();
}

void updateCameraMode(unsigned long currentMillis) {
    // Por ahora no hacemos nada
    return;

}

void handleExpressionChange() {
    if (server.hasArg("type")) {
        String expr = server.arg("type");
        RobotExpression newExpr = EXPR_NORMAL;
        
        if (expr == "happy") newExpr = EXPR_HAPPY;
        else if (expr == "sad") newExpr = EXPR_SAD;
        else if (expr == "surprised") newExpr = EXPR_SURPRISED;
        else if (expr == "sleeping") newExpr = EXPR_SLEEPING;
        else if (expr == "look_up") newExpr = EXPR_LOOK_UP;
        else if (expr == "look_down") newExpr = EXPR_LOOK_DOWN;
        else if (expr == "look_left") newExpr = EXPR_LOOK_LEFT;
        else if (expr == "look_right") newExpr = EXPR_LOOK_RIGHT;
        
        robotEyes->setExpression(newExpr);
        neopixelManager->setColorForExpression(newExpr);
        
        server.send(200, "text/plain", "OK");
    } else {
        server.send(400, "text/plain", "Missing type parameter");
    }
}

void handleThemeChange() {
    if (server.hasArg("theme")) {
        String theme = server.arg("theme");
        ClockTheme newTheme = THEME_KAWAII;
        
        if (theme == "minimal") newTheme = THEME_MINIMAL;
        else if (theme == "neon") newTheme = THEME_NEON;
        else if (theme == "space") newTheme = THEME_SPACE;
        else if (theme == "korean") newTheme = THEME_KOREAN;
        
        clockDisplay->setTheme(newTheme);
        neopixelManager->setColorForClockTheme(newTheme);
        
        server.send(200, "text/plain", "OK");
    } else {
        server.send(400, "text/plain", "Missing theme parameter");
    }
}

void handleModeChange() {
    if (server.hasArg("mode")) {
        String mode = server.arg("mode");
        if (mode == "robot") currentMode = MODE_ROBOT;
        else if (mode == "clock") currentMode = MODE_CLOCK;
        else if (mode == "camera") currentMode = MODE_CAMERA;
        
        server.send(200, "text/plain", "OK");
    } else {
        server.send(400, "text/plain", "Missing mode parameter");
    }
}

void setupWebServer() {
    // Página principal
    server.on("/", HTTP_GET, []() {
        String html = "<html><head>";
        html += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
        html += "<style>";
        html += "body { font-family: Arial; margin: 20px; text-align: center; background: #f0f0f0; }";
        html += ".container { max-width: 800px; margin: 0 auto; background: white; padding: 20px; border-radius: 10px; box-shadow: 0 2px 5px rgba(0,0,0,0.1); }";
        html += "button { margin: 10px; padding: 15px 25px; font-size: 16px; cursor: pointer; background: #4CAF50; color: white; border: none; border-radius: 5px; }";
        html += "button:hover { background: #45a049; }";
        html += ".section { margin: 20px 0; padding: 15px; border: 1px solid #ddd; border-radius: 5px; }";
        html += "</style></head><body>";
        html += "<div class='container'>";
        html += "<h1>Robot Control Panel</h1>";
        
        // Sección de expresiones
        html += "<div class='section'>";
        html += "<h2>Expressions</h2>";
        html += "<button onclick='setExpression(\"normal\")'>Normal</button>";
        html += "<button onclick='setExpression(\"happy\")'>Happy</button>";
        html += "<button onclick='setExpression(\"sad\")'>Sad</button>";
        html += "<button onclick='setExpression(\"surprised\")'>Surprised</button>";
        html += "</div>";
        
        // Sección de temas del reloj
        html += "<div class='section'>";
        html += "<h2>Clock Themes</h2>";
        html += "<button onclick='setTheme(\"kawaii\")'>Kawaii</button>";
        html += "<button onclick='setTheme(\"minimal\")'>Minimal</button>";
        html += "<button onclick='setTheme(\"neon\")'>Neon</button>";
        html += "<button onclick='setTheme(\"space\")'>Space</button>";
        html += "<button onclick='setTheme(\"korean\")'>Korean</button>";
        html += "</div>";
        
        // Sección de modos
        html += "<div class='section'>";
        html += "<h2>Modes</h2>";
        html += "<button onclick='setMode(\"robot\")'>Robot Mode</button>";
        html += "<button onclick='setMode(\"clock\")'>Clock Mode</button>";
        html += "<button onclick='setMode(\"camera\")'>Camera Mode</button>";
        html += "</div>";
        
        // JavaScript para controles
        html += "<script>";
        html += "function setExpression(type) {";
        html += "  fetch('/api/expression?type=' + type)";
        html += "    .then(response => console.log('Expression changed'))";
        html += "    .catch(error => console.error('Error:', error));";
        html += "}";
        html += "function setTheme(theme) {";
        html += "  fetch('/api/theme?theme=' + theme)";
        html += "    .then(response => console.log('Theme changed'))";
        html += "    .catch(error => console.error('Error:', error));";
        html += "}";
        html += "function setMode(mode) {";
        html += "  fetch('/api/mode?mode=' + mode)";
        html += "    .then(response => console.log('Mode changed'))";
        html += "    .catch(error => console.error('Error:', error));";
        html += "}";
        html += "</script>";
        html += "</div></body></html>";
        server.send(200, "text/html", html);
    });

    // API endpoints
    server.on("/api/expression", HTTP_GET, handleExpressionChange);
    server.on("/api/theme", HTTP_GET, handleThemeChange);
    server.on("/api/mode", HTTP_GET, handleModeChange);
    
    server.begin();
}

void setup() {
    Serial.begin(115200);
    
    // Inicializar pantalla
    tft.begin();
    tft.setRotation(1);
    tft.fillScreen(GC9A01A_BLACK);

    // Inicializar NeoPixel
    pixels.begin();
    neopixelManager = new NeoPixelManager(pixels);

    // Inicializar clases principales
    robotEyes = new RobotEyes(tft);
    clockDisplay = new ClockDisplay(tft);
   // camera = new CameraManager();

    // Configurar pin táctil
    pinMode(TOUCH_PIN, INPUT);

    // Inicializar WiFi
    WiFi.begin(ssid, password);
    Serial.print("Conectando a WiFi");

    // Mostrar mensaje de inicio
    tft.setTextSize(2);
    tft.setTextColor(GC9A01A_WHITE);
    tft.setCursor(20, 100);
    tft.print("Conectando WiFi...");
    
    // Esperar conexión WiFi con timeout
    int wifiAttempts = 0;
    while (WiFi.status() != WL_CONNECTED && wifiAttempts < 20) {
        delay(500);
        Serial.print(".");
        wifiAttempts++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        Serial.println("\nConectado a WiFi");
        Serial.print("IP: ");
        Serial.println(WiFi.localIP());

        // Configurar tiempo NTP
        configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
        
        // Mostrar IP en pantalla
        tft.fillScreen(GC9A01A_BLACK);
        tft.setCursor(20, 90);
        tft.print("IP: ");
        tft.println(WiFi.localIP());
        delay(3000);
        
        // Inicializar servidor web
        setupWebServer();
    } else {
        Serial.println("\nFallo al conectar al WiFi");
        tft.fillScreen(GC9A01A_BLACK);
        tft.setCursor(20, 90);
        tft.print("WiFi no conectado");
        delay(2000);
    }

    robotEyes->setExpression(EXPR_NORMAL);
}

void loop() {
    unsigned long currentMillis = millis();
    
    // Manejar servidor web
    server.handleClient();
    
    // Actualizar NeoPixel
    neopixelManager->update();
    
    // Manejar sensor táctil
    handleTouchSensor(currentMillis);
    
    // Actualizar según el modo actual
    switch (currentMode) {
        case MODE_ROBOT:
            updateRobotMode(currentMillis);
            break;
        case MODE_CLOCK:
            updateClockMode(currentMillis);
            break;
        case MODE_CAMERA:
            updateCameraMode(currentMillis);
            break;
    }
}