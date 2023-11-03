#include <Arduino.h>

#include "wifi_esp.h"
#include "mqtt_esp.h"
#include "config.h"

void setup() {
    Serial.begin(115200);

    set_parameters_wifi(WIFI_SSID, WIFI_PASSWORD);
    set_parameters_mqtt(MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD);

    xTaskCreate(wifi_loop, "wifi_manager", 16384, NULL, 1, NULL);
    delay(1000);
    xTaskCreate(mqtt_loop, "mqtt_manager", 16384, NULL, 1, NULL);
}

void loop() {
    
}
