/*
  # Author : Watchara Pongsri
  # [github/X-c0d3] https://github.com/X-c0d3/
  # Web Site: https://www.rockdevper.com
*/

#ifndef WIFIMAN_H
#define WIFIMAN_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <DNSServer.h>
#include <FS.h>
#include <WiFi.h>

#include "SPIFFS.h"
#ifdef ESP32
#include <SPIFFS.h>
#endif
#include "settings.h"

bool shouldSaveConfig = true;
void saveConfigCallback() {
    Serial.println("Should save config");
    shouldSaveConfig = true;
}

char socketio_server[40];
char socketio_port[6];
char line_api_key[60];
char firebase_host[60];
char firebase_api_key[60];

void wifiReset() {
    Serial.println("Erase settings and restart ...");
    delay(1000);

    WiFi.disconnect(true);  // erases store credentially
    // wifiManager.resetSettings();
    SPIFFS.format();  // erases stored values
    ESP.restart();
}

void setup_Wifi(uint8_t led_pin) {
    // Serial.println(String("WIFI_SSID: ") + WIFI_SSID);
    // Serial.println(String("WIFI_PASSWORD: ") + WIFI_PASSWORD);

    // WiFi.useStaticBuffers(true);
    WiFi.mode(WIFI_STA);
    WiFi.setAutoReconnect(true);
    delay(100);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    WiFi.setTxPower(WIFI_POWER_8_5dBm);

    delay(100);
    Serial.println();
    printMessage(0, 1, "WIFI Connecting...", true);

    int i = 0;
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(String(i) + ",");
        if (i == 40) {
            Serial.print("ESP Restarting ");
            ESP.restart();
        }
        digitalWrite(led_pin, !digitalRead(led_pin));
        i++;
    }

    Serial.println();
    Serial.print("WIFI Connected ");
    String ip = WiFi.localIP().toString();
    Serial.println(ip.c_str());

    long rssi = WiFi.RSSI();
    Serial.println("WifiSignal: " + String(rssi) + "db");

    Serial.println("Socket.io Server: " + String(SOCKETIO_HOST));
    Serial.println();
}

long rssi;
long wifiSignal() {
    // WifiSignalStrength
    // Convert to scale -48 to 0 eg. map(rssi, -100, 0, 0, -48);
    // I used -100 instead of -120 because <= -95 is unusable
    // Negative number so we can draw n pixels from the bottom in black
    rssi = WiFi.RSSI();  // eg. -63
    if (rssi < -99) {
        rssi = -99;
    }
    // Serial.println("WifiSignal: " + String(rssi) + "db");
    return rssi;
}

#endif