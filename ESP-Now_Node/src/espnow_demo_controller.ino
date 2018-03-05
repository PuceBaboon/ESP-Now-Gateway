/*
 *   $Id: espnow_demo_controller.ino,v 1.1 2018/02/16 14:32:52 gaijin Exp $
 *
 *  Controller.  I send messages to the "Slave" and then go to sleep for a while.
 *               The data sent is random crap, just to test the gateway.
 *
 *  From: https://lowreal.net/2016/01/14/2
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
extern "C" {
    #include <espnow.h>
    #include <user_interface.h>
    #include <espnow_common.h>
}

#define	MY_LOC_ID	27
uint8_t mac[] = { 0x02, 0x60, 0x0D, 0xF0, 0x0D, 0x02 };

void printMacAddress(uint8_t * macaddr) {
    for (int i = 0; i < 6; i++) {
        Serial.print(macaddr[i], HEX);
        if (i < 5)
            Serial.print(':');
    }
    Serial.println();
}

void setup() {
    pinMode(13, OUTPUT);

    Serial.begin(115200);
    Serial.println("Initializing...");
    WiFi.mode(WIFI_STA);

    uint8_t macaddr[6];
    wifi_get_macaddr(STATION_IF, macaddr);
    Serial.print("mac address (STATION_IF): ");
    printMacAddress(macaddr);

    wifi_get_macaddr(SOFTAP_IF, macaddr);
    Serial.print("mac address (SOFTAP_IF): ");
    printMacAddress(macaddr);

    if (esp_now_init() == 0) {
        Serial.println("direct link  init ok");
    } else {
        Serial.println("dl init failed");
        ESP.restart();
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);
    esp_now_register_recv_cb([]
                             (uint8_t * macaddr, uint8_t * data,
                              uint8_t len) {
                             Serial.println("recv_cb");
                             Serial.print("mac address: ");
                             printMacAddress(macaddr);
                             Serial.print("data: ");
                             for (int i = 0; i < len; i++) {
                             Serial.print(data[i], HEX);}
                             Serial.println("");});
    esp_now_register_send_cb([](uint8_t * macaddr, uint8_t status) {
                             Serial.println("send_cb");
                             Serial.print("mac address: ");
                             printMacAddress(macaddr);
                             Serial.print("status = ");
                             Serial.println(status);
                             });

    int res = esp_now_add_peer(wifi_mac, (uint8_t) ESP_NOW_ROLE_SLAVE,
                               (uint8_t) WIFI_DEFAULT_CHANNEL, NULL, 0);


    sensorData.loc_id = MY_LOC_ID;
    sensorData.temp = ((uint8_t) RANDOM_REG32);
    sensorData.humidity = ((uint8_t) RANDOM_REG32);
    sensorData.pressure = ((uint8_t) RANDOM_REG32);

    uint8_t bs[sizeof(sensorData)];
    memcpy(bs, &sensorData, sizeof(sensorData));
    esp_now_send(wifi_mac, bs, sizeof(sensorData));

    ESP.deepSleep(20e6, WAKE_RF_DEFAULT);
}

void loop() {
}
