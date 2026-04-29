#pragma once
#include <Arduino.h>
#include <Preferences.h>

class Pair {
  public:
    static void saveMAC(uint8_t macArray[6]);
    static void loadMAC(uint8_t macArray[6]);
    static bool hasSavedMAC();

    static esp_err_t addPeer(uint8_t* peerMAC);
  private:
};