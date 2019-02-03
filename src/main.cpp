#define FW_NAME "esp12e"
#define FW_VERSION "0.2.0"

#include "Homie.h"

const int PIN_LED = 2;

void onHomieEvent(const HomieEvent& event) {
  switch(event.type) {
    case HomieEventType::MQTT_READY:
      Homie.getLogger() << "MQTT connected, preparing for deep sleep..." << endl;
      Homie.prepareToSleep();
      break;
    case HomieEventType::READY_TO_SLEEP:
      Homie.getLogger() << "Ready to sleep" << endl;
      Homie.doDeepSleep(60*1000000, RFMode::RF_DISABLED);
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  // WiFi does not connect properly after being woken up from deep sleep.
  // Disconnecting solves this issue.
  WiFi.disconnect();

  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie.setLedPin(PIN_LED, LOW);
  Homie.disableResetTrigger();

  Homie.onEvent(onHomieEvent);

  Homie.setup();
}

void loop() {
  Homie.loop();
}
