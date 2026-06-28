Read data from LVTOPSUN-512314-G4 LFP Battery via ESP32-C3

```
Example: JSON output
{
    "deviceName": "LVTOPSUN_BATTERY",
    "deviceId": "500a75a65688",
    "lastUpdated": "31/5/2026 18:9:14",
    "ipAddress": "192.168.137.104",
    "wifiSignal": -81,
    "firmware_version": "1.0.5",
    "firmware_lastupdate": "2026-01-03",
    "deviceState": {
        "isValid": true,
        "packVoltage": 52.73300171,
        "current": 0,
        "soc": 54,
        "remainingAh": 169.5599976,
        "fullAh": 314,
        "designAh": 314,
        "cycleCount": 0,
        "maxCellTemp": 31.20000076,
        "minCellTemp": 30.70000076,
        "cellTempDiff": 0.5,
        "mosTemperature": 31.60000038,
        "ambientTemperature": 35.09999847,
        "cells": [
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.292999983,
            3.290999889
        ],
        "cellTemperatures": [
            31,
            30.70000076,
            30.70000076,
            31.20000076
        ]
    }
}

```

<p float="center">
<img src="https://github.com/X-c0d3/lvtopsun-battery-monitoring-esp32/blob/main/doc/image1.jpg?v=2"  width="ุ600">
<img src="https://github.com/X-c0d3/lvtopsun-battery-monitoring-esp32/blob/main/doc/image2.jpg"  width="ุ600">
<img src="https://github.com/X-c0d3/lvtopsun-battery-monitoring-esp32/blob/main/doc/image3.jpg"  width="ุ600">
<img src="https://github.com/X-c0d3/lvtopsun-battery-monitoring-esp32/blob/main/doc/image4.jpg"  width="ุ600">
</p>

HA Config

```
type: custom:jk-bms-card
language: auto
prefix: jk_bms
cellCount: 16
cellColumns: 4
cellLayout: bankMode
deltaVoltageUnit: V
resistanceUnit: Ω
tempSensorsCount: 4
hasHeater: "1"
socDecimals: 0
customDecimals: 2
showTitle: true
showButtons: true
showMain: true
showCondensed: true
showCells: true
showResistances: true
showCardVersion: true
entities:
  delta_cell_voltage: sensor.jk_bms_delta_cell_voltage
  balancing: ""
  balancing_current: ""
  power: ""
  balance_trigger_voltage: ""
  balance_starting_voltage: ""
  total_runtime_formatted: ""
  charging: sensor.jk_bms_charging
  charging_power: sensor.jk_bms_charging_power
  discharging: sensor.jk_bms_discharging
  discharging_power: sensor.jk_bms_discharging_power
  balancer: ""
  heater: ""
  total_voltage: sensor.jk_bms_total_voltage
  total_battery_capacity_setting: ""
  total_charging_cycle_capacity: sensor.jk_bms_total_charging_cycle_capacity
  average_cell_voltage: ""
  current: sensor.jk_bms_current
  state_of_charge: sensor.jk_bms_state_of_charge
  capacity_remaining: sensor.jk_bms_remaining_capacity
  charging_cycles: sensor.jk_bms_cycle_count
  power_tube_temperature: sensor.jk_bms_mos_temperature
  min_voltage_cell: ""
  max_voltage_cell: ""
  min_cell_voltage: ""
  max_cell_voltage: ""
  errors: ""
  software_version: ""
  hardware_version: ""
  cell_voltage_1: sensor.jk_bms_cell_voltage_01
  cell_voltage_2: sensor.jk_bms_cell_voltage_02
  cell_voltage_3: sensor.jk_bms_cell_voltage_03
  cell_voltage_4: sensor.jk_bms_cell_voltage_04
  cell_voltage_5: sensor.jk_bms_cell_voltage_05
  cell_voltage_6: sensor.jk_bms_cell_voltage_06
  cell_voltage_7: sensor.jk_bms_cell_voltage_07
  cell_voltage_8: sensor.jk_bms_cell_voltage_08
  cell_voltage_9: sensor.jk_bms_cell_voltage_09
  cell_voltage_10: sensor.jk_bms_cell_voltage_10
  cell_voltage_11: sensor.jk_bms_cell_voltage_11
  cell_voltage_12: sensor.jk_bms_cell_voltage_12
  cell_voltage_13: sensor.jk_bms_cell_voltage_13
  cell_voltage_14: sensor.jk_bms_cell_voltage_14
  cell_voltage_15: sensor.jk_bms_cell_voltage_15
  cell_voltage_16: sensor.jk_bms_cell_voltage_16
  temperature_sensor_1: sensor.jk_bms_cell_temperature_1
  temperature_sensor_2: sensor.jk_bms_cell_temperature_2
  temperature_sensor_3: sensor.jk_bms_cell_temperature_3
  temperature_sensor_4: sensor.jk_bms_cell_temperature_4
title: LVTOPSUN 314A 16Kw
layout: core-reactor
cellColorMode: progress
cellOrientation: horizontal
minCellVoltage: 3
maxCellVoltage: 3.65
batteryName: lvtopson
showMainStats: true
```
