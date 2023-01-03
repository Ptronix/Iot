Der folgende Code sollte verwendet werden, um eine ESP32-Cam mit einem Webserver zu programmieren, der über das Internet erreichbar ist.

#include <WiFi.h>
#include <WebServer.h>
#include <ESP32Camera.h>

// Konfigurieren Sie WiFi
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

// Konfigurieren Sie den Webserver
WebServer server(80);

// Konfigurieren Sie die Kamera
#define CAMERA_MODEL_AI_THINKER
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

// Kameraobjekt erstellen
ESP32Camera cam(CAMERA_MODEL_AI_THINKER, PWDN_GPIO_NUM, RESET_GPIO_NUM, XCLK_GPIO_NUM, SIOD_GPIO_NUM, SIOC_GPIO_NUM, Y9_GPIO_NUM, Y8_GPIO_NUM, Y7_GPIO_NUM, Y6_GPIO_NUM, Y5_GPIO_NUM, Y4_GPIO_NUM, Y3_GPIO_NUM, Y2_GPIO_NUM, VSYNC_GPIO_NUM, HREF_GPIO_NUM, PCLK_GPIO_NUM);

// Bildvariablen
uint8_t *fb = NULL;
uint32_t _frameSize;

void setup() {
  Serial.begin(115200);
  
  // Wifi verbinden
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Verbindung zum Wifi wird hergestellt..");
  }
  Serial.println("Verbunden!");
  Serial.println(WiFi.localIP());
  
  // Kamera initialisieren
  cam.init();
  _frameSize = cam.getImageSize();
  fb = (uint8_t *)malloc(_frameSize);
  
  // Webserver einrichten
  server.on("/", handleRoot);
  server.begin();
  Serial.println("Webserver gestartet!");
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  cam.run();
  cam.getFB(fb);
  server.send_P(200, "image/jpeg", fb, _frameSize);
}
