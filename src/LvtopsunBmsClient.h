/*
  # Author : Watchara Pongsri
  # [github/X-c0d3] https://github.com/X-c0d3/
  # Web Site: https://www.rockdevper.com
*/

#ifndef LVTOPSUN_BMS_CLIENT_H
#define LVTOPSUN_BMS_CLIENT_H

#include <Arduino.h>

#include "models/BmsData.h"
#include "utility.h"

class LvtopsunBmsClient {
   private:
    HardwareSerial& serialPort;
    uint32_t baudRate;
    uint8_t rxPin;
    uint8_t txPin;

    String rxBuffer = "";
    unsigned long lastDataTime = 0;
    const unsigned long rxTimeout = 200;

    long parseHexSub(const String& str, int start, int length) {
        String sub = str.substring(start, start + length);
        return strtol(sub.c_str(), NULL, 16);
    }

    String generatePacePacket(String cid2, String lengthAndData) {
        String protocolVersion = "25";
        String address = "01";
        String cid1 = "46";

        String asciiPayload = protocolVersion + address + cid1 + cid2 + lengthAndData;

        int sum = 0;
        for (int i = 0; i < asciiPayload.length(); i++) {
            sum += asciiPayload.charAt(i);
        }

        int lrcVal = (~sum + 1) & 0xFFFF;
        char lrcHex[5];
        sprintf(lrcHex, "%04X", lrcVal);

        return "~" + asciiPayload + String(lrcHex) + "\r";
    }

   public:
    // Constructor
    LvtopsunBmsClient(HardwareSerial& serial, uint8_t rx, uint8_t tx, uint32_t baud = 9600)
        : serialPort(serial), rxPin(rx), txPin(tx), baudRate(baud) {}

    bool connect() {
        serialPort.begin(baudRate, SERIAL_8N1, rxPin, txPin);
        Serial.println("LvtopsunBmsClient serial initialized");
        return true;
    }

    void disconnect() {
        serialPort.end();
    }

    void sendRefreshCommand() {
        if (!serialPort) return;
        String finalPacket = generatePacePacket("42", "E00201");
        serialPort.print(finalPacket);
    }

    bool handleStream(BmsData& outData) {
        while (serialPort.available()) {
            char c = serialPort.read();
            rxBuffer += c;
            lastDataTime = millis();
        }

        if (rxBuffer.length() > 0 && (millis() - lastDataTime >= rxTimeout)) {
            bool success = decodeTelemetry(rxBuffer, outData);
            rxBuffer = "";
            return success;
        }
        return false;
    }

    bool decodeTelemetry(String rawStr, BmsData& data) {
        rawStr.trim();

        if (!rawStr.startsWith("~") || rawStr.length() < 150) return false;

        String rtnStatus = rawStr.substring(7, 9);
        if (rtnStatus != "00") return false;

        try {
            data.cells.clear();
            data.cellTemperatures.clear();

            int idx = 17;

            // 1. แกะแรงดันรายเซลล์
            int cellCount = parseHexSub(rawStr, idx, 2);
            idx += 2;

            for (int i = 0; i < cellCount; i++) {
                float cellVolt = parseHexSub(rawStr, idx, 4) / 1000.0f;
                data.cells.push_back(cellVolt);
                idx += 4;
            }

            // 2. แกะอุณหภูมิทั้งหมด 6 จุด
            int tempCount = parseHexSub(rawStr, idx, 2);
            idx += 2;

            float rawTemps[6] = {0.0f};
            for (int t = 0; t < tempCount && t < 6; t++) {
                rawTemps[t] = (parseHexSub(rawStr, idx, 4) - 2731) / 10.0f;
                idx += 4;
            }

            for (int i = 0; i < 4; i++) {
                data.cellTemperatures.push_back(rawTemps[i]);
            }
            data.mosTemperature = rawTemps[4];
            data.ambientTemperature = rawTemps[5];

            data.maxCellTemp = max(max(rawTemps[0], rawTemps[1]), max(rawTemps[2], rawTemps[3]));
            data.minCellTemp = min(min(rawTemps[0], rawTemps[1]), min(rawTemps[2], rawTemps[3]));
            data.cellTempDiff = data.maxCellTemp - data.minCellTemp;

            // 3. แกะกระแสไฟฟ้า และแรงดันไฟฟ้ารวม
            long currentRaw = parseHexSub(rawStr, idx, 4);
            if (currentRaw > 0x7FFF) currentRaw -= 0x10000;
            data.current = currentRaw / 100.0f;
            idx += 4;

            data.packVoltage = parseHexSub(rawStr, idx, 4) / 1000.0f;
            idx += 4;

            idx += 6;  // ข้ามกลุ่มบิตสถานะความปลอดภัย และ Warning Flag Count

            // 4. แกะข้อมูลความจุและค่าเปอร์เซ็นต์ SoC ท้ายเฟรม
            data.designAh = parseHexSub(rawStr, idx, 4) / 100.0f;
            idx += 4;

            data.cycleCount = parseHexSub(rawStr, idx, 4);
            idx += 4;

            data.fullAh = parseHexSub(rawStr, idx, 4) / 100.0f;
            idx += 4;

            data.soc = (float)parseHexSub(rawStr, idx, 2);
            data.remainingAh = (data.fullAh * data.soc) / 100.0f;

            data.isValid = true;
            return true;

        } catch (...) {
            data.isValid = false;
            return false;
        }
    }

    void printBmsConsole(const BmsData& data) {
        unsigned long startTime = micros();

        if (!data.isValid) return;

        Serial.println("\n======================================================");
        Serial.println("⚡ [LVTopsun BMS Telemetry Data - ONLINE]");
        Serial.println("======================================================");

        // แสดงผลแรงดันรายเซลล์แบบ Vector รองรับขนาดไดนามิก
        for (size_t i = 0; i < data.cells.size(); i++) {
            Serial.printf("🔋 Cell %02d : %.3f V\n", (i + 1), data.cells[i]);
        }

        Serial.println("------------------------------------------------------");

        // จัดรูปแบบการพ่น String ของ PACK Cell Temp
        Serial.print("🌡️  PACK Cell Temp    : ");
        for (size_t i = 0; i < data.cellTemperatures.size(); i++) {
            Serial.printf("[%d] %.1f°C", (i + 1), data.cellTemperatures[i]);
            if (i < data.cellTemperatures.size() - 1) Serial.print(" | ");
        }
        Serial.println("");

        Serial.printf("🌡️  Max/Min Temp Diff : Max: %.1f°C | Min: %.1f°C | Diff: %.1f°C\n", data.maxCellTemp, data.minCellTemp, data.cellTempDiff);
        Serial.printf("🔥 MOS Temperature   : %.1f°C\n", data.mosTemperature);
        Serial.printf("🌬️  Ambient Temp     : %.1f°C\n", data.ambientTemperature);
        Serial.println("------------------------------------------------------");

        Serial.printf("📊 Pack Voltage      : %.3f V\n", data.packVoltage);
        Serial.printf("🔌 Current           : %.2f A\n", data.current);
        Serial.printf("🔋 Battery Capacity  : %.2f Ah / %.2f Ah (Design: %.0f Ah)\n", data.remainingAh, data.fullAh, data.designAh);
        Serial.printf("🔄 Battery SoC       : %.1f%% (Cycle Count: %ld รอบ)\n", data.soc, data.cycleCount);
        Serial.println("======================================================");

        unsigned long elapsedTime = micros() - startTime;
        Serial.println(" -------------- ElapsedTime " + formatDuration(elapsedTime) + " -------------- Last Update: " + DateNowString());
    }
};

#endif