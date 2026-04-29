#include "Pair.hpp"
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