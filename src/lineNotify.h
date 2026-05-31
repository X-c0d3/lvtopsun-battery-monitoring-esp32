/*
  # Author : Watchara Pongsri
  # [github/X-c0d3] https://github.com/X-c0d3/
  # Web Site: https://www.rockdevper.com
*/

#ifndef LINE_NOTIFY_H
#define LINE_NOTIFY_H

#include <Arduino.h>
#include <WiFiClientSecure.h>

#include "settings.h"

void Line_Notify(String message) {
    if (!ENABLE_LINE_NOTIFY)
        return;

    WiFiClientSecure client;
    client.setInsecure();
    client.setTimeout(API_TIMEOUT);
    if (!client.connect("notify-api.line.me", 443)) {
        Serial.println("connection failed");
        return;
    }
    Serial.println("Send Line-Notify");

    String req = "";
    req += "POST /api/notify HTTP/1.1\r\n";
    req += "Host: notify-api.line.me\r\n";
    req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
    req += "Cache-Control: no-cache\r\n";
    req += "User-Agent: ESP8266\r\n";
    req += "Content-Type: application/x-www-form-urlencoded\r\n";
    req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
    req += "\r\n";
    req += "message=" + message;
    Serial.println(req);
    client.print(req);
    delay(20);

    if (ENABLE_DEBUG_MODE)
        Serial.println(req);

    Serial.println("-------------");
    while (client.connected()) {
        String line = client.readStringUntil('\n');
        if (line == "\r") {
            break;
        }

        if (ENABLE_DEBUG_MODE)
            Serial.println(line);
    }
    Serial.println("-------------");
}

#endif