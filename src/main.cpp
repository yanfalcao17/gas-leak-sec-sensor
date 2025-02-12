#include <Arduino.h>
#include <WiFi.h>
#include <WebSocketsClient.h>

#define SSID_ESP "GAS-LEAK-SENSOR:200"
#define PASSWORD "" // Senha vazia

#define LED_PIN 2
#define BUZZER_PIN 23

WebSocketsClient webSocket;

// Controle do alarme
void alarmON()
{
    digitalWrite(LED_PIN, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);
    delay(5000);
}

void alarmOFF()
{
    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);
}

// Lidar com mensagens recebidas do WebSocket
void webSocketEvent(WStype_t type, uint8_t *payload, size_t length)
{
    switch (type)
    {
    case WStype_CONNECTED:
        Serial.println("✅ Conectado ao WebSocket Server!");
        webSocket.sendTXT("ESP32 conectado!");
        break;

    case WStype_TEXT:
        Serial.printf("📩 Mensagem recebida: %s\n", payload);

        if (strcmp((char *)payload, "on") == 0)
        {
            alarmON();
        }
        else if (strcmp((char *)payload, "off") == 0)
        {
            alarmOFF();
        }
        break;

    case WStype_DISCONNECTED:
        Serial.println("❌ Desconectado do WebSocket Server! Tentando reconectar...");
        break;
    }
}

// Conectar-se ao Wi-Fi
void connectWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(SSID_ESP, PASSWORD);

    Serial.print("🔄 Conectando-se ao Wi-Fi...");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("\n✅ Conectado ao Wi-Fi!");
    Serial.print("📡 IP: ");
    Serial.println(WiFi.localIP());
}

// Inicializar WebSocket Client
void initWebSocket()
{
    webSocket.begin("192.168.4.200", 81, "/");
    webSocket.onEvent(webSocketEvent);
    webSocket.setReconnectInterval(5000); // Tenta reconectar a cada 5 segundos
}

void setup()
{
    Serial.begin(115200);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    alarmOFF(); // Garante que o alarme comece desligado

    connectWiFi();
    initWebSocket();
}

void loop()
{
    webSocket.loop();
}