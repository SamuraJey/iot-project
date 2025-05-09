#include <ADXL345.h>
#include <Wire.h>  // TODO Проверить, возможно она нам не нужна.

#include "Arduino.h"
#include "EspMQTTClient.h"
#include "consts.h"
#include "secrets.h"

EspMQTTClient client(
    WIFI_SSID,
    WIFI_PASSWORD,
    MQTT_SERVER,
    MQTT_CLIENT_ID);

ADXL345 adxl;

// Состояние телеграмм-сигнализации по умолчанию
bool guard = true;

void setup() {
    Serial.begin(115200);
    // Инициализация ADXL345
    adxl.powerOn();
    // Дебаг сообщения MQTT в консоль.
    client.enableDebuggingMessages();
}

void guardCallback(const String& payload) {
    guard = payload[0] - '0';
    Serial.print("guard set to: ");
    Serial.println(guard);
}

void onConnectionEstablished() {
    client.subscribe("base/relay/led1", guardCallback);
}

String generateMessage() {
    String message = "Movement detected:";
    return message;
}

bool checkForMovement() {
    double cur[3];
    adxl.getAcceleration(cur);
    static double prev[3] = cur;
    bool ret = ((cur[0] - prev[0]) * (cur[0] - prev[0]) +
                (cur[1] - prev[1]) * (cur[1] - prev[1]) +
                (cur[2] - prev[2]) * (cur[2] - prev[2])) >
               MOVEMENT_THRESHOLD_SQUARE;
    memcpy(prev, cur, sizeof(prev));
    return ret;
}

void loop() {
    // runs once per loop call
    client.loop();  // Обслуживание MQTT
    unsigned long curTime = millis();

    static unsigned long probe_CD = curTime;
    if (curTime < probe_CD) return;
    probe_CD += PROBE_DELAY;
    // runs once per PROBE_DELAY

    static unsigned long msg_CD = 0;
    if (!checkForMovement() || curTime < msg_CD) return;
    msg_CD = curTime + MSG_DELAY;
    // runs once trigger
    Serial.println("Triggered");

    if (guard) client.publish("base/sensor/movement", generateMessage());
    return;
}