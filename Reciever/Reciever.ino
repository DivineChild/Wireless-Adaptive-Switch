#include <esp_now.h>
#include <WiFi.h>
#include <Wireless-Adaptive-Switch.hpp>

typedef struct struct_message {
  Commands::command code;
} struct_message;

struct_message myData;

const int BUTTON = 15;

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

if (myData.code == Commands::PAIR && !Pair::hasSavedMAC()) {
  Serial.println("Discover ping");
  
  uint8_t senderMAC[6];
  memcpy(senderMAC, info->src_addr, 6);

  if (!esp_now_is_peer_exist(senderMAC)) {
    Pair::saveMAC(senderMAC);
    Pair::addPeer(senderMAC);
  }

  // Now reply with reciever MAC
  uint8_t myMac[6];
  WiFi.macAddress(myMac);
  Serial.println(esp_now_send(senderMAC, myMac, 6));
} else if (myData.code == Commands::ON) {
    digitalWrite(BUTTON, HIGH);
  } else if (myData.code == Commands::OFF) {
    digitalWrite(BUTTON, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  if (Pair::hasSavedMAC()) {
    uint8_t mac;
    Pair::loadMAC(&mac);
    Util::macToString(&mac);

    Pair::addPeer(&mac);
  }

  esp_now_register_recv_cb(OnDataRecv);

  pinMode(BUTTON, OUTPUT);
}

void loop() {
}
