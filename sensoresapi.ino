#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

// Configuración de la red WiFi
const char* ssid = "redlocal2.4g";
const char* password = "Mena180602";

// Configuración del servidor web
ESP8266WebServer server(80);

// Configuración del sensor DHT11
#define DHTPIN D1  // Pin donde está conectado el DHT11
#define DHTTYPE DHT11  // Tipo de sensor DHT
DHT dht(DHTPIN, DHTTYPE);

// Configuración del sensor de ultrasonido HC-SR04
const int echoPin = D2; // Pin del echo del sensor HC-SR04
const int trigPin = D3; // Pin del trigger del sensor HC-SR04

void setup() {
  // Inicialización de la conexión WiFi
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Inicialización del servidor web
  server.on("/temperatura", HTTP_GET, [](){
    float temperatura = dht.readTemperature();
    if (isnan(temperatura)) {
      server.send(500, "text/plain", "Error obteniendo la temperatura");
    } else {
      server.send(200, "text/plain", String(temperatura));
    }
  });

  server.on("/proximidad", HTTP_GET, [](){
    // Medición de proximidad con el sensor HC-SR04
    long duration, distance;
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distance = (duration / 2) / 29.1; // Convertir el tiempo en distancia en centímetros

    server.send(200, "text/plain", String(distance));
  });

  server.begin();
  Serial.println("HTTP server started");

  // Inicialización del sensor DHT11
  dht.begin();

  // Inicialización del sensor HC-SR04
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  server.handleClient();
}
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}
