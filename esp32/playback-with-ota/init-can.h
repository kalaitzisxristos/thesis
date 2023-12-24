#include "Arduino.h"
#include "CAN.h"
#include "js-async.h"

namespace init_can {
  struct CanData {
    int32_t time;
    bool isExtended;
    uint32_t id;
    int32_t size;
    uint64_t data;
  };

  int logIdx = 1;
  unsigned long logInitTime;
  struct CanData log[] = {
    { .time = -1 }
    // #include "./can-log.19.h"
  };
  const int logLength = sizeof(log) / sizeof(CanData);

  void loop();

  int can_baud = 1000E3;

  void init_can() {
    while (!CAN.begin(can_baud)) {
      Serial.println(F("Attempting to connect to CAN bus ... "));
      delay(100);
    }

    logInitTime = millis();
    setInterval([] () {
      unsigned long dt = millis() - logInitTime;
      struct CanData current = log[logIdx];
      if (dt < current.time - log[1].time) { return; }
      Serial.print("Sending packet at index: ");
      Serial.print(logIdx);
      Serial.println(" ...");

      if (current.isExtended) {
        CAN.beginExtendedPacket(current.id);
      } else {
        CAN.beginPacket(current.id);
      }

      byte b;

      if (current.size >= 8) {
        b = (byte)(current.data >> 0x38);
        CAN.write(b);
        Serial.print(b, HEX); Serial.print(" ");
      }

      if (current.size >= 7) {
        b = (byte)(current.data >> 0x30);
        CAN.write(b);
        Serial.print(b, HEX); Serial.print(" ");
      }

      if (current.size >= 6) {
        b = (byte)(current.data >> 0x28);
        CAN.write(b);
        Serial.print(b, HEX); Serial.print(" ");
      }

      if (current.size >= 5) {
        b = (byte)(current.data >> 0x20);
        CAN.write(b);
        Serial.print(b, HEX); Serial.print(" ");
      }

      if (current.size >= 4) {
        b = (byte)(current.data >> 0x18);
        CAN.write(b);
        Serial.print(b, HEX); Serial.print(" ");
      }

      if (current.size >= 3) {
        b = (byte)(current.data >> 0x10);
        CAN.write(b);
        Serial.print(b, HEX); Serial.print(" ");
      }

      if (current.size >= 2) {
        b = (byte)(current.data >> 0x08);
        CAN.write(b);
        Serial.print(b, HEX); Serial.print(" ");
      }

      if (current.size >= 1) {
        b = (byte)(current.data >> 0x00);
        CAN.write(b);
        Serial.print(b, HEX); Serial.print(" ");
      }

      CAN.endPacket();
      Serial.print('\n');

      logIdx += 1;
      if (logIdx >= logLength) {
        logIdx = 1;
        logInitTime = millis();
      }
    }, 0);
  }

}