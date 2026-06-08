/*
  # Author : Watchara Pongsri
  # [github/X-c0d3] https://github.com/X-c0d3/
  # Web Site: https://www.rockdevper.com
*/

#ifndef LVTOPSUN_BMS_DATA_H
#define LVTOPSUN_BMS_DATA_H

#include <Arduino.h>
#include <ArduinoJson.h>

#include <vector>

class BmsData {
   public:
    bool isValid = false;

    // Volt & Current
    float packVoltage = 0.0f;
    float current = 0.0f;
    float energy = 0.0f;
    bool isCharging = false;
    bool isDischarging = false;

    // Capacity & SoC
    float soc = 0.0f;
    float soh = 0.0f;
    float remainingAh = 0.0f;
    float fullAh = 0.0f;
    float designAh = 0.0f;
    long cycleCount = 0;

    // Temperatures
    std::vector<float> cellTemperatures;
    float maxCellTemp = 0.0f;
    float minCellTemp = 0.0f;
    float cellTempDiff = 0.0f;
    float mosTemperature = 0.0f;
    float ambientTemperature = 0.0f;

    String status = "";

    // Cell Voltages (16 Cells)
    std::vector<float> cells;

    /**
     * แปลงข้อมูล BMS ทั้งหมดให้อยู่ในรูปแบบ JsonObject (ArduinoJson)
     */
    void toJson(JsonObject obj) const {
        obj["isValid"] = isValid;
        obj["packVoltage"] = packVoltage;
        obj["current"] = current;
        obj["energy"] = energy;
        obj["isCharging"] = isCharging;
        obj["isDischarging"] = isDischarging;
        obj["soc"] = soc;
        obj["soh"] = soh;
        obj["remainingAh"] = remainingAh;
        obj["fullAh"] = fullAh;
        obj["designAh"] = designAh;
        obj["cycleCount"] = cycleCount;
        obj["status"] = status;

        obj["maxCellTemp"] = maxCellTemp;
        obj["minCellTemp"] = minCellTemp;
        obj["cellTempDiff"] = cellTempDiff;
        obj["mosTemperature"] = mosTemperature;
        obj["ambientTemperature"] = ambientTemperature;

        // แปลงเวกเตอร์แรงดันไฟฟ้ารายเซลล์ (cells Array -> JsonArray)
        JsonArray cellsArray = obj.createNestedArray("cells");
        for (float volt : cells) {
            cellsArray.add(volt);
        }

        // แปลงเวกเตอร์อุณหภูมิเซลล์ (cellTemperatures Array -> JsonArray)
        JsonArray tempArray = obj.createNestedArray("cellTemperatures");
        for (float temp : cellTemperatures) {
            tempArray.add(temp);
        }
    }
};

#endif