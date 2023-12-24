#include "Arduino.h"
#include "js-async.h"

namespace init_blink {
  bool led = false;

  void init_blink() {
    pinMode(LED_BUILTIN, OUTPUT);

    setInterval([]() {
      led = !led;
      digitalWrite(LED_BUILTIN, led);
    }, 100);
  }
}