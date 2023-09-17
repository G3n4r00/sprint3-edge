#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h> // Para trabalhar com dados JSON

const char* ssid = "ssid";
const char* password = "senha";
const char* serverUrl = "https://url-do-painel.com/api/ponto-de-dados";

const int trigPin = 22;
const int echoPin = 23;
const int pirPin = 24;
const int buzzerPin = 25;

long duration;
int distance;
bool isPersonDetected = false;

void setup() {
  Serial.begin(115200);

  // Inicializa o Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao Wi-Fi...");
  }
  Serial.println("Conectado ao Wi-Fi");
}

void loop() {
  isPersonDetected = digitalRead(pirPin);

  if (isPersonDetected) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = duration * 0.034 / 2;

    // Cria um payload JSON com os dados do sensor
    StaticJsonDocument<200> jsonPayload;
    jsonPayload["distancia"] = distance;

    // Envia os dados para o servidor
    HTTPClient http;
    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/json");

    // Serializa o payload JSON
    String jsonString;
    serializeJson(jsonPayload, jsonString);

    int httpResponseCode = http.POST(jsonString);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(response);
    } else {
      Serial.print("Erro ao enviar dados para o servidor. Código de erro HTTP: ");
      Serial.println(httpResponseCode);
    }

    http.end();

    // Executa ações com base na distância
    if (distance >= 13) {
      tone(buzzerPin, 700, 7000); // Ajuste a frequência e a duração conforme necessário
    } else {
      noTone(buzzerPin);
    }
  } else {
    noTone(buzzerPin);
  }

  delay(1000);
}
