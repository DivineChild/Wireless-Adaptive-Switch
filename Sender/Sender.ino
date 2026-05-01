/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>
#include <Wireless-Adaptive-Switch.hpp>

uint8_t peerMAC[6];

// Discover MAC Address
uint8_t broadcastAddress[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  Commands::command code;
} struct_message;

bool paired = false;
unsigned long lastBroadcast = 0;
const int BROADCAST_INTERVAL = 500; // ms

int buttonPIN = 15;

// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

bool lastWrite = false;

// callback when data is sent
void OnDataSent(const wifi_tx_info_t *info, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

// When you get a response, set paired = true and save the MAC
void OnDataRecv(const esp_now_recv_info_t *info, const uint8_t *data, int len) {
  Serial.println("Data Recv");
  for (int i = 0; i < len; i++) {
    Serial.println(data[i]);
  }
  if (!paired) {
    memcpy(peerMAC, info->src_addr, 6);
    Serial.printf("Pair: %s\n", Util::macToString(peerMAC));
    paired = true;
    Serial.println("Paired!");
    Pair::saveMAC(peerMAC);
    Pair::loadMAC(peerMAC);

    // Register Peer
    Pair::addPeer(peerMAC);
  }
}

void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

  Pair::clearSettings();
 
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  pinMode(buttonPIN, INPUT);


  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  paired = Pair::hasSavedMAC();


  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Transmitted packet
  esp_now_register_send_cb(OnDataSent);
  esp_now_register_recv_cb(OnDataRecv);

  if (paired) {
    Pair::loadMAC(peerMAC);

    Util::macToString(peerMAC);

    Pair::addPeer(peerMAC);
  } else {
    // Register and add peer        
    if (Pair::addPeer(broadcastAddress) != ESP_OK){
      Serial.println("Failed to add peer");
      return;
    }
  }
}
 
void loop() {
  if (!paired && millis() - lastBroadcast > BROADCAST_INTERVAL) {
    myData.code = Commands::PAIR;
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
    lastBroadcast = millis();
    Serial.println("Broadcasting...");
  } else if (paired) {
    lastWrite = !lastWrite;

    // Set values to send
    myData.code = digitalRead(buttonPIN) == LOW ? Commands::OFF : Commands::ON;
    
    Serial.printf("MAC: %s\n", Util::macToString(peerMAC));

    // Send message via ESP-NOW
    esp_err_t result = esp_now_send(peerMAC, (uint8_t *) &myData, sizeof(myData));
    
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }
    delay(20);
  }
}