#define FW_NAME "esp12e"
#define FW_VERSION "0.1.0"

#include "Homie.h"

const int PIN_LED = 2;

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie.setLedPin(PIN_LED, LOW);
  Homie.disableResetTrigger();

  Homie.setup();
}

void loop() {
  Homie.loop();
}
