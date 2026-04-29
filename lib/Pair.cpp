#include "Pair.hpp"
#include <esp_now.h>
static Preferences prefs;


void Pair::saveMAC(uint8_t macArray[6]) {
  prefs.begin("wireless-button", false);
  prefs.putBytes("peer_mac", macArray, 6);
  prefs.end();
}

void Pair::loadMAC(uint8_t macArray[6]) {
  prefs.begin("wireless-button", true);
  prefs.getBytes("peer_mac", macArray, 6);
  prefs.end();
}

bool Pair::hasSavedMAC() {
  prefs.begin("wireless-button", true);
  bool exists = prefs.isKey("peer_mac");
  prefs.end();
  return exists;
}

esp_err_t Pair::addPeer(uint8_t* peerMAC) {
  esp_now_peer_info_t newPeer = {};
  memcpy(newPeer.peer_addr, peerMAC, 6);
  newPeer.channel = 0;
  newPeer.encrypt = false;
  return esp_now_add_peer(&newPeer);
} 