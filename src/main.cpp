#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <DHT.h>

#include "wifi_esp.h"
#include "mqtt_esp.h"
#include "config.h"

#define DHTPIN 16

DHT dht(DHTPIN, DHT11);

void read_DHT11(void * pvParameters) {
    float t;
    float h;

    dht.begin();
    vTaskDelay(1000/portTICK_PERIOD_MS );

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for(;;) {
        t = dht.readTemperature();
        if (isnan(t)) {
            Serial.println("Failed to read temperature from DHT sensor!");
        } else {
            Serial.printf("Temperatura: %.1f°C\n", t);
        }

        h = dht.readHumidity();
        if (isnan(h)) {
            Serial.println("Failed to read humidity from DHT sensor!");
        } else {
            Serial.printf("Umidade: %.1f%\n", h);
        }

        vTaskDelayUntil(&xLastWakeTime, 1000/portTICK_PERIOD_MS );
    }
}

void setup() {
    Serial.begin(115200);

    set_parameters_wifi(WIFI_SSID, WIFI_PASSWORD);
    set_parameters_mqtt(MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD);

    xTaskCreate(wifi_loop, "wifi_manager", 16384, NULL, 1, NULL);
    delay(1000);
    xTaskCreate(mqtt_loop, "mqtt_manager", 16384, NULL, 1, NULL);
    delay(500);
    xTaskCreate(read_DHT11, "DHT11", 16384, NULL, 1, NULL);
}

void loop() {
    
}
