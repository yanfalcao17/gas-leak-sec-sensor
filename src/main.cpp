// #include <WiFi.h>
// #include <esp_wifi.h>

// //código para descobrir o MAC do ESP32
// void readMacAddress(){
//   uint8_t baseMac[6];
//   esp_err_t ret = esp_wifi_get_mac(WIFI_IF_STA, baseMac);
//   if (ret == ESP_OK) {
//     Serial.printf("%02x:%02x:%02x:%02x:%02x:%02x\n",
//                   baseMac[0], baseMac[1], baseMac[2],
//                   baseMac[3], baseMac[4], baseMac[5]);
//   } else {
//     Serial.println("Failed to read MAC address");
//   }
// }

// void setup(){
//   Serial.begin(115200);

//   WiFi.mode(WIFI_STA);
//   WiFi.begin();

//   Serial.print("[DEFAULT] ESP32 Board MAC Address: ");
//   readMacAddress();
// }
 
// void loop(){

// }


#include <esp_now.h>
#include <WiFi.h>

int buzzerPin = 21;

// Callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
    int command;
    memcpy(&command, incomingData, sizeof(command));
    Serial.print("Command received: ");
    Serial.println(command);
    
    if (command == 1) {
        digitalWrite(buzzerPin, HIGH);
        delay(2000);
        digitalWrite(buzzerPin, LOW);
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(buzzerPin, OUTPUT);
    WiFi.mode(WIFI_STA);

    if (esp_now_init() != ESP_OK) {
        Serial.println("Error initializing ESP-NOW");
        return;
    }
    esp_now_register_recv_cb(OnDataRecv);
}

void loop() {

}