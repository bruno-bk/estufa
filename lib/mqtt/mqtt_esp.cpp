#include "mqtt_esp.h"
#include "wifi_esp.h"
#include "config.h"

#include <PubSubClient.h>

extern QueueHandle_t lux;
extern QueueHandle_t temperature;
extern QueueHandle_t pressure;
extern QueueHandle_t wind_speed;
extern QueueHandle_t humidity;
extern QueueHandle_t angle;

extern WiFiClient wifi_esp;
PubSubClient client_mqtt(wifi_esp);

char mqtt_server[100] = "";
int mqtt_port = 1883;
char mqtt_user[50];
char mqtt_password[50];

void connect_broker() {
    client_mqtt.setServer(mqtt_server, mqtt_port);

    while (!client_mqtt.connected()) {
        Serial.print("Attempting MQTT connection to ");
        Serial.println(mqtt_server);
        if (client_mqtt.connect("Estufa")) {
            Serial.println("connected");
            vTaskDelay(1000/portTICK_PERIOD_MS);

        } else {
            Serial.print("failed, rc=");
            Serial.print(client_mqtt.state());
            Serial.println(" try again in 5 seconds");
            vTaskDelay(5000/portTICK_PERIOD_MS);
        }
    }
}

boolean client_mqtt_is_connected(){
    return client_mqtt.connected();
}

void send_mqtt_message(const char* topic, const char* msg) {
    Serial.print("[");
    Serial.print(millis());
    Serial.print("] send: ");
    Serial.print(msg);
    Serial.print(" to ");
    Serial.println(topic);
    client_mqtt.publish(topic, msg);
}

void set_parameters_mqtt(const char* server, int port, const char* user, const char* password) {
    strcpy(mqtt_server, server);
    mqtt_port = port;
    strcpy(mqtt_user, user);
    strcpy(mqtt_password, password);
}

void send_messages_to_broker() {
    return;
}

void mqtt_loop(void *pvParameters) {
    TickType_t xLastWakeTime = xTaskGetTickCount();

    for(;;) {
        if (client_mqtt.connected()) {
            send_messages_to_broker();
        } else {
            connect_broker();
        }

        client_mqtt.loop();

        vTaskDelayUntil(&xLastWakeTime, 100/portTICK_PERIOD_MS );
    }
}

