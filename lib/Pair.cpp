#include "Pair.hpp"
#include <esp_now.h>
#include "Util.hpp"
static Preferences prefs;


void Pair::saveMAC(uint8_t macArray[6]) {
	prefs.begin("wswitch", false);
  prefs.putBytes("peer_mac", macArray, 6);
  Serial.printf("Saving: %s\n", Util::macToString(macArray));
  prefs.end();
}

void Pair::loadMAC(uint8_t macArray[6]) {
  prefs.begin("wswitch", true);
  prefs.getBytes("peer_mac", macArray, 6);
  Serial.printf("Loading: %s\n", Util::macToString(macArray));
  prefs.end();
}

void Pair::clearSettings() {
  prefs.clear();
	prefs.remove("peer_mac");
}

bool Pair::hasSavedMAC() {
  prefs.begin("wswitch", true);
  bool exists = prefs.isKey("peer_mac");
  prefs.end();

  if (!exists) return false;

  uint8_t mac[6];
  loadMAC(mac);

  Serial.print("Loaded MAC: ");
  Serial.println(Util::macToString(mac));

  for (int i = 0; i < 6; i++) {
    if (mac[i] != 0x00) return true;
  }
  return false;
}

esp_err_t Pair::addPeer(uint8_t* peerMAC) {
  esp_now_peer_info_t newPeer = {};
  memcpy(newPeer.peer_addr, peerMAC, 6);
  newPeer.channel = 0;
  newPeer.encrypt = false;
  return esp_now_add_peer(&newPeer);
} 
