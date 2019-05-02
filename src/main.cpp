#define FW_NAME "esp8266-12e"
#define FW_VERSION "0.6.4"

#include "Homie.h"
// #include "AdcNode.hpp"
#include "DHT22Node.hpp"
#include "DS18B20Node.hpp"
#include "Timer.h"

const int PIN_LED = 2;
const int PIN_DHT = 13;
const int PIN_DS = 4;

DS18B20Node ds18b20("ds18b20", PIN_DS, 60);
DHT22Node dht22("dht22", PIN_DHT, 60);
// AdcNode adc("adc");
Timer t;

bool upgrading = false;

void prepareSleep() {
  Homie.getLogger() << "Prepare to sleep." << endl;
  Homie.prepareToSleep();
}

void onHomieEvent(const HomieEvent& event) {
  switch(event.type) {
    case HomieEventType::MQTT_DISCONNECTED:
      Homie.getLogger() << "MQTT disconnected, reason: " << (int8_t)event.mqttReason << endl;
      break;
    case HomieEventType::MQTT_READY:
      t.after(100, prepareSleep);
      break;
    case HomieEventType::OTA_STARTED:
      upgrading = true;
      break;
    case HomieEventType::OTA_FAILED:
    case HomieEventType::OTA_SUCCESSFUL:
      upgrading = false;
      break;
    case HomieEventType::READY_TO_SLEEP:
      if (upgrading) {
        t.after(100, prepareSleep);
      } else {
        Homie.doDeepSleep(60*1000000);
      }
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  Serial << endl << endl;

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_DHT, INPUT);
  pinMode(PIN_DS, INPUT_PULLUP);

  // WiFi does not connect properly after being woken up from deep sleep.
  // Trying different things to solve this.
  WiFi.forceSleepWake();

  Homie_setFirmware(FW_NAME, FW_VERSION);
  Homie.setLedPin(PIN_LED, LOW);
  Homie.disableResetTrigger();

  Homie.onEvent(onHomieEvent);

//   adc.beforeHomieSetup();

  Homie.setup();
}

void loop() {
  Homie.loop();
  t.update();
}
