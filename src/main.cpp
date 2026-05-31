/*
  # Author : Watchara Pongsri
  # [github/X-c0d3] https://github.com/X-c0d3/
  # Web Site: https://www.rockdevper.com
*/

#include <HardwareSerial.h>
#include <SocketIoClient.h>
#include <arduino-timer.h>

#include "LvtopsunBmsClient.h"  // Import คลาสโมดูลใหม่เข้าโปรเจกต์
#include "broadCastToClients.h"
#include "models/BmsData.h"

// #include "broadCastToClients.h"
#include "esp_task_wdt.h"
#include "utility.h"
#include "wifiMan.h"
#define RX_PIN 20
#define TX_PIN 21
#define ESP32C3_LED 8  // Built-in LED
#define WDT_TIMEOUT 60

HardwareSerial hwSerial(1);
SocketIoClient webSocket;

LvtopsunBmsClient bms(Serial1, RX_PIN, TX_PIN, 9600);
BmsData bmsDataObj;
auto timer = timer_create_default();  // create a timer with default settings

unsigned long lastRefreshTime = 0;
const unsigned long refreshInterval = 3000;

void event(const char* payload, size_t length) {
    // Serial.println("Socket.io Event Received:");
    // Serial.println(String(payload).substring(0, length));
}

void setup() {
    Serial.begin(115200);
    pinMode(ESP32C3_LED, OUTPUT);

    // Connect WIFI
    setup_Wifi(ESP32C3_LED);

    setupTimeZone();

    if (bms.connect()) {
        Serial.println("✅ Connected to LVTopsun BMS");
    } else {
        Serial.println("❌ Connection failed");
    }

    if (WiFi.status() == WL_CONNECTED) {
        delay(500);
        bms.sendRefreshCommand();

        if (ENABLE_SOCKETIO) {
            webSocket.begin(SOCKETIO_HOST, SOCKETIO_PORT);
            webSocket.on(SOCKETIO_CHANNEL, event);
        }

        esp_task_wdt_init(WDT_TIMEOUT, true);  // true = reset chip
        esp_task_wdt_add(NULL);
    }
}

void loop() {
    if (WiFi.status() == WL_CONNECTED) {
        webSocket.loop();
        esp_task_wdt_reset();
    }

    // สั่งส่งสัญญาณรีเฟรชนำร่องทุก 2 วินาที (Non-blocking)
    unsigned long currentMillis = millis();
    if (currentMillis - lastRefreshTime >= refreshInterval) {
        lastRefreshTime = currentMillis;
        bms.sendRefreshCommand();
    }

    // ปล่อยให้ระบบคลาสจัดการจัดการรวบรวมบัฟเฟอร์สตริงดิบ
    if (bms.handleStream(bmsDataObj)) {
        // print response to console
        bms.printBmsConsole(bmsDataObj);

        yield();
        digitalWrite(ESP32C3_LED, !digitalRead(ESP32C3_LED));
        publishToSocketIO(webSocket, bmsDataObj);
    }

    timer.tick();
}