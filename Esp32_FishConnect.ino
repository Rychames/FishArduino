#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "Familia Xames";
const char* password = "xames2022@#";

const int ledPin = 2; // LED integrado no ESP32
const char* deviceName = "ESP32_Xames"; // Nome do dispositivo
unsigned long startTime; // Para armazenar o tempo de funcionamento
int ledState = LOW; // Estado inicial do LED
int turnOffCount = 0; // Contador de quantas vezes o LED foi desligado

AsyncWebServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  // Conectar ao Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi");

  // Armazenar o tempo de início
  startTime = millis();
  
  // Rota para ligar o LED
  server.on("/ligar", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, HIGH);
    ledState = HIGH;
    request->send(200, "text/plain", "LED Ligado");
  });

  // Rota para desligar o LED
  server.on("/desligar", HTTP_GET, [](AsyncWebServerRequest *request){
    digitalWrite(ledPin, LOW);
    ledState = LOW;
    turnOffCount++; // Incrementa o contador
    request->send(200, "text/plain", "LED Desligado");
  });

  // Rota para enviar as informações em JSON
  server.on("/info", HTTP_GET, [](AsyncWebServerRequest *request){
    String jsonResponse = "{";
    jsonResponse += "\"name\":\"" + String(deviceName) + "\",";
    jsonResponse += "\"uptime\":\"" + String((millis() - startTime) / 1000) + "\",";
    jsonResponse += "\"state\":\"" + String(ledState == HIGH ? "Ligado" : "Desligado") + "\",";
    jsonResponse += "\"count_off\":" + String(turnOffCount);
    jsonResponse += "}";

    request->send(200, "application/json", jsonResponse);
  });

  server.begin();
}

void loop() {
  // Nada a fazer aqui
}
