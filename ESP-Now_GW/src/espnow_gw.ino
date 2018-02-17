/*
 *   $Id: espnow_gw.ino,v 1.5 2018/02/16 15:39:04 gaijin Exp $
 *
 * ESP-Now WiFi to Eternet Gateway.  Takes packets from other ESP-Nodes
 * and publishes them to MQTT via a cheap, readily available and easily
 * interfaceable W5500 Ethernet board.
 *
 * The intended use of this project is to enable multiple (up to 20)
 * battery-powered ESP8266 boards to support various sensors around the
 * home, sending data back to the main network via this gateway.  The use
 * of ESP-Now should greatly extend the battery life of the ESP8266 nodes
 * beyond that of normal WiFi.  ESP-Now communicates using low-level
 * protocols (using the module's MAC address instead of requiring an IP),
 * thus shortening the time during which the node needs to consume 
 * relatively large amounts of power for radio transmissions.
 *
 * This gateway is intended to be mains powered and permanently linked to
 * a LAN via the W5500 board.  The ESP-Now connection is via the WiFi, which
 * -never- operates in normal "station" or "access-point" mode; it only
 * processes the short, MAC addressed, ESP-Now protocol messages.
 *
 * ESP-Now - Note that the Espressif NON-OS API reference manual has this
 *           information on the connection between ESP-Now roles and WiFi 
 *           modes:-
 *
 *             - "Slave" or "Combo" roles should use either SoftAP or
 *               SoftAP + Station mode.
 *             - "Controller" role should use Station mode.
 *
 * In ESP-Now parlance this, the gateway, is a "Slave", whilst the
 * multiple battery-powered nodes are "Controllers".  Strange!
 *
 */

#include <SPI.h>
#include <Ethernet.h>           // Required for ETHERNET. Wiznet code in lib dir.
#include <PubSubClient.h>
#include <ESP8266WiFi.h>        // Required for WIFI. Normal, built-in library.
/* *INDENT-OFF* */
extern "C" {
    #include <espnow.h>
    #include <user_interface.h>
    #include "espnow_common.h"
    #include "espnow_gw.h"
}
/* *INDENT-ON* */ 


/*
 * Display numeric MAC address in standard format.
 */
void printMacAddress(uint8_t * macaddr) {
    for (int i = 0; i < 6; i++) {
        Serial.print(macaddr[i], HEX);
        if (i < 5)
            Serial.print(':');
    }
    Serial.println();
}


/*
 * Set up ESP-Now WiFi, role and call-backs.
 */
void initEspNow() {

    /*
     * Initialize the on-board WiFi before starting ESP-Now.
     */
    wifi_set_opmode(STATIONAP_MODE);    // Recommended mode.
    wifi_set_macaddr(SOFTAP_IF, &wifi_mac[0]);  // Set our special MAC address.
    wifi_set_channel(WIFI_DEFAULT_CHANNEL);     // Set our specific channel.
    WiFi.disconnect(true);

    if (esp_now_init() != 0) {
        Serial.println("*** ESP_Now init failed");
        ESP.restart();
    }

    Serial.print("This node AP mac: ");
    Serial.println(WiFi.softAPmacAddress());
    Serial.print("This node STA mac: ");
    Serial.println(WiFi.macAddress());

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);

/* *INDENT-OFF* */
    esp_now_register_recv_cb([](uint8_t * r_mac, uint8_t * r_data, uint8_t len) {
                 memcpy(&rec_MAC, r_mac, sizeof(rec_MAC));	// Save MAC passed in cb.
                 memcpy(&sensorData, r_data, sizeof(sensorData));	// Save data struct passed in cb.
                 haveReading = true;}
    );
/* *INDENT-ON* */

    /*
     * Brute-force add of our peers to the peer list.
     */
    (void *) esp_now_add_peer(cont01_mac,
                              (uint8_t) ESP_NOW_ROLE_CONTROLLER,
                              (uint8_t) WIFI_DEFAULT_CHANNEL, NULL, 0);
    (void *) esp_now_add_peer(cont02_mac,
                              (uint8_t) ESP_NOW_ROLE_CONTROLLER,
                              (uint8_t) WIFI_DEFAULT_CHANNEL, NULL, 0);
    (void *) esp_now_add_peer(cont03_mac,
                              (uint8_t) ESP_NOW_ROLE_CONTROLLER,
                              (uint8_t) WIFI_DEFAULT_CHANNEL, NULL, 0);
    (void *) esp_now_add_peer(cont04_mac,
                              (uint8_t) ESP_NOW_ROLE_CONTROLLER,
                              (uint8_t) WIFI_DEFAULT_CHANNEL, NULL, 0);
    (void *) esp_now_add_peer(cont05_mac,
                              (uint8_t) ESP_NOW_ROLE_CONTROLLER,
                              (uint8_t) WIFI_DEFAULT_CHANNEL, NULL, 0);
    (void *) esp_now_add_peer(cont06_mac,
                              (uint8_t) ESP_NOW_ROLE_CONTROLLER,
                              (uint8_t) WIFI_DEFAULT_CHANNEL, NULL, 0);
}


/*
 * Change this for the specific reset sequence required for
 * your particular board or module.
 */
void WizReset() {
    Serial.println("Starting Wiz W5500 Ethernet...");
    pinMode(RESET_P, OUTPUT);
    digitalWrite(RESET_P, HIGH);
    delay(250);
    digitalWrite(RESET_P, LOW);
    delay(50);
    digitalWrite(RESET_P, HIGH);
    delay(350);
}


void mq_callback(char *topic, byte * payload, unsigned int length) {
    Serial.print("Message arrived [");
    Serial.print(topic);
    Serial.print("] ");
    for (int i = 0; i < length; i++) {
        Serial.print((char) payload[i]);
    }
    Serial.println();
}


void mq_publish(const char *topic, const char *payload) {
    Serial.print("Publishing... ");
    if (client.publish(topic, payload)) {
        Serial.print(" OK ");
    } else {
        Serial.print(" FAILED ");
    }
    Serial.print(" topic: ");
    Serial.print(topic);
    Serial.print(" payload: ");
    Serial.println(payload);
}


void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("arduinoClient")) {
            Serial.println("connected");
            // Once connected, publish an announcement...
            client.publish("outTopic", "Hello world!");
            // ...and resubscribe
            client.subscribe("inTopic");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" .  Trying again in 5 seconds...");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
}


void setup() {
    Serial.begin(115200);
    delay(500);
    Serial.println("\n\n\nEthernet-Based MQTT Gateway v1.0\n\n");

    /*
     * Start the ESP-Now listener on the WiFi network.
     */
    initEspNow();

    client.setServer(mq_server, 1883);
    client.setCallback(mq_callback);

    /*
     * Start the Ethernet (wired, non-ESPNOW) connection and the server.
     */
    WizReset();

    /* 
     * Network configuration - all except the MAC are optional.
     *
     * IMPORTANT NOTE - The mass-produced W5500 boards do -not-
     *                  have a built-in MAC address (depite 
     *                  comments to the contrary elsewhere). You
     *                  -must- supply a MAC address here.
     */
    Ethernet.begin(eth_MAC, eth_IP, eth_DNS, eth_GW, eth_MASK);

    delay(200);

    Serial.print("Ethernet IP is: ");
    Serial.println(Ethernet.localIP());
    }


void loop() {
    char buff[MQ_TOPIC_MAX];

    if (haveReading) {
        haveReading = false;
        Serial.print("Connection from: ");
        printMacAddress(rec_MAC);
        Serial.printf("  Id: %i, Temp=%0.1f, Hum=%0.0f%%, pressure=%0.0fmb\r\n",
                      sensorData.loc_id, sensorData.temp, sensorData.humidity,
                      sensorData.pressure);
        snprintf(buff, MQ_TOPIC_MAX, "%i:%0.2f:%0.0f:%0.0f", sensorData.loc_id, sensorData.temp, sensorData.humidity, sensorData.pressure);
        mq_publish((char *)TOPIC01, buff);
    }
    if (!client.connected()) {
              reconnect();
    }
    client.loop();
    yield();
}
