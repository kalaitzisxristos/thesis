#include <WiFi.h>
#include <WiFiClient.h>
// #include <ESPmDNS.h>

#include "js-async.h"

#include "./init-server.h"
#include "./init-blink.h"
#include "./init-can.h"

const char* host = "esp32";
const char* ssid = "WIFI NAME";
const char* password = "WIFI PASSWORD";

/*
 * setup function
 */
void setup(void) {
  Serial.begin(115200);

  // Connect to WiFi network
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  /*use mdns for host name resolution*/
  // if (!MDNS.begin(host)) { //http://esp32.local
  //   Serial.println("Error setting up MDNS responder!");
  //   while (1) {
  //     delay(1000);
  //   }
  // }
  // Serial.println("mDNS responder started");

  init_server::init_server();

  async::CurrentTime = millis;
  init_blink::init_blink();
  init_can::init_can();

  setInterval([]() { init_server::server.handleClient(); }, 1);
}

void loop(void) { async::InvokeAll(); }
