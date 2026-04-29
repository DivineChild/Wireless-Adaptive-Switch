#include <esp_now.h>
#include <WiFi.h>
#include "../Sender/Commands.hpp"

typedef struct struct_message {
  Commands::command code;
} struct_message;

struct_message myData;

// Receive callback - correct signature for core 3.x
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *incomingData, int len) {
  if (len != sizeof(myData)) {
    Serial.println("Ignoring non-command message");
    return;
  }
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.println(myData.code);

if (myData.code == Commands::PAIR) {
  Serial.println("Discover ping");
  
  uint8_t senderMAC[6];
  memcpy(senderMAC, info->src_addr, 6);

  // Register sender as a peer first
  esp_now_peer_info_t peerInfo = {};
  memcpy(peerInfo.peer_addr, senderMAC, 6);
  peerInfo.channel = 0;
  peerInfo.encrypt = false;

  if (!esp_now_is_peer_exist(senderMAC)) {
    esp_now_add_peer(&peerInfo);
  }

  // Now reply with our MAC
  uint8_t myMac[6];
  WiFi.macAddress(myMac);
  Serial.println(esp_now_send(senderMAC, myMac, 6));
} else if (myData.code == Commands::ON) {
    digitalWrite(2, HIGH);
  } else if (myData.code == Commands::OFF) {
    digitalWrite(2, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  pinMode(2, OUTPUT);
}

void loop() {
}
