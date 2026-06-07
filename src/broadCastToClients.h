/*
  # Author : Watchara Pongsri
  # [github/X-c0d3] https://github.com/X-c0d3/
  # Web Site: https://www.rockdevper.com
*/

#ifndef BROADCAST_TO_CLIENTS_H
#define BROADCAST_TO_CLIENTS_H

#include <Arduino.h>
#include <ArduinoJson.h>
#include <SocketIoClient.h>

#include "lineNotify.h"
#include "settings.h"
#include "utility.h"
#include "wifiMan.h"

void publishToSocketIO(SocketIoClient& webSocket, const BmsData& data) {
    // https://arduinojson.org/v6/assistant/
    unsigned long startTime = micros();
    StaticJsonDocument<1024> root;
    root["deviceName"] = DEVICE_NAME;
    root["deviceId"] = getChipId();
    root["lastUpdated"] = DateNowString();
    root["ipAddress"] = WiFi.localIP().toString();
    root["wifiSignal"] = wifiSignal();
    root["firmware_version"] = String(FIRMWARE_VERSION);
    root["firmware_lastupdate"] = String(FIRMWARE_LASTUPDATE);

    JsonObject deviceState = root.createNestedObject("deviceState");
    data.toJson(deviceState);

    String output;
    serializeJsonPretty(root, output);

    // Publish to socket.io server
    if (ENABLE_SOCKETIO) {
        webSocket.emit(SOCKETIO_CHANNEL, output.c_str());
        unsigned long elapsedTime = micros() - startTime;
        Serial.print(">>> Socket.IO Emit ElapsedTime: ");
        Serial.println(formatDuration(elapsedTime));
    }

    if (ENABLE_DEBUG_MODE)
        Serial.print(output);
}

#endif