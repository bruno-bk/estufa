#include "mqtt_esp.h"
#include "wifi_esp.h"
#include "config.h"

#include <ESP32Servo.h>

#include <PubSubClient.h>

extern QueueHandle_t temperature;
extern QueueHandle_t humidity;
extern QueueHandle_t hygrometer;

extern WiFiClient wifi_esp;
PubSubClient client_mqtt(wifi_esp);

char mqtt_server[100] = "";
int mqtt_port = 1883;
char mqtt_user[50];
char mqtt_password[50];

#define REDPIN 27
#define GRENPIN 14
#define BLUEPIN 26
#define COOLERPIN 13
#define HUMIDIFIERPIN 12
#define SERVOPIN 23

Servo servoMotor;

void callback(char* topic, byte* message, unsigned int length) {
    message[length] = '\0';

    if (String(topic) == "greenhouse/lighting") {
        Serial.print("Iluminação = ");
        Serial.println(atoi((char *)message));
        analogWrite(REDPIN, atoi((char *)message));
        analogWrite(GRENPIN, (atoi((char *)message) * .1));
        analogWrite(BLUEPIN, atoi((char *)message));
    }
    if (String(topic) == "greenhouse/cooler") {
        Serial.print("Cooler = ");
        Serial.println(atoi((char *)message));
        analogWrite(COOLERPIN, atoi((char *)message));
    }
    if (String(topic) == "greenhouse/nebulizer") {
        Serial.print("Nebulizador = ");
        Serial.println(atoi((char *)message));
        analogWrite(HUMIDIFIERPIN, atoi((char *)message));
    }
    if (String(topic) == "greenhouse/ventilation") {
        Serial.print("Servo = ");
        Serial.println(atoi((char *)message));
        servoMotor.write(atoi((char *)message));
    }
}

void connect_broker() {
    while (!client_mqtt.connected()) {
        Serial.print("Attempting MQTT connection to ");
        Serial.println(mqtt_server);
        if (client_mqtt.connect("Estufa")) {
            Serial.println("connected");

            client_mqtt.subscribe("greenhouse/lighting");
            client_mqtt.subscribe("greenhouse/cooler");
            client_mqtt.subscribe("greenhouse/nebulizer");
            client_mqtt.subscribe("greenhouse/ventilation");

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
    float read_value;
    uint16_t read_value_int;
    char message[10];

    if (xQueueReceive(temperature, &read_value, pdMS_TO_TICKS(0)) == true) {
        sprintf(message, "%d", (uint32_t)read_value);
        send_mqtt_message("greenhouse/temperature", message);
    }

    if (xQueueReceive(humidity, &read_value, pdMS_TO_TICKS(0)) == true) {
        sprintf(message, "%.1f", read_value);
        send_mqtt_message("greenhouse/humidity", message);
    }

    if (xQueueReceive(hygrometer, &read_value_int, pdMS_TO_TICKS(0)) == true) {
        sprintf(message, "%d", read_value_int);
        send_mqtt_message("greenhouse/hygrometer", message);
    }
}

void mqtt_loop(void *pvParameters) {
    servoMotor.setPeriodHertz(50); 
    servoMotor.attach(SERVOPIN, 600, 2400);

    client_mqtt.setServer(mqtt_server, mqtt_port);
    client_mqtt.setCallback(callback);

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

