#include <Arduino.h>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

#include <DHT.h>

#include "wifi_esp.h"
#include "mqtt_esp.h"
#include "config.h"

QueueHandle_t temperature;
QueueHandle_t humidity;
QueueHandle_t hygrometer;

#define DHTPIN 16
#define HYGPIN 32

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
            xQueueSend(temperature, &t, pdMS_TO_TICKS(0));
        }

        h = dht.readHumidity();
        if (isnan(h)) {
            Serial.println("Failed to read humidity from DHT sensor!");
        } else {
            xQueueSend(humidity, &h, pdMS_TO_TICKS(0));
        }

        vTaskDelayUntil(&xLastWakeTime, 1000/portTICK_PERIOD_MS );
    }
}

void read_hygrometer(void * pvParameters) {
    uint16_t h;

    TickType_t xLastWakeTime = xTaskGetTickCount();

    for(;;) {
        h = analogRead(HYGPIN);
        xQueueSend(hygrometer, &h, pdMS_TO_TICKS(0));

        vTaskDelayUntil(&xLastWakeTime, 1000/portTICK_PERIOD_MS );
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(HYGPIN, INPUT);

    set_parameters_wifi(WIFI_SSID, WIFI_PASSWORD);
    set_parameters_mqtt(MQTT_SERVER, MQTT_PORT, MQTT_USER, MQTT_PASSWORD);

    temperature = xQueueCreate(10, sizeof(float));
    humidity = xQueueCreate(10, sizeof(float));
    hygrometer = xQueueCreate(10, sizeof(uint16_t));

    xTaskCreate(wifi_loop, "wifi_manager", 16384, NULL, 1, NULL);
    delay(1000);
    xTaskCreate(mqtt_loop, "mqtt_manager", 16384, NULL, 1, NULL);
    delay(500);
    xTaskCreate(read_DHT11, "DHT11", 16384, NULL, 1, NULL);
    delay(500);
    xTaskCreate(read_hygrometer, "hygrometer", 16384, NULL, 1, NULL);
}

void loop() {

}
