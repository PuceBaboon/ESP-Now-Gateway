/*
 *   $Id: espnow_gw.h,v 1.4 2018/02/17 00:03:54 gaijin Exp $
 *
 * Defines specific to the ESP-Now Gateway.
 */
#ifndef __ESPNOW_GW__
#define __ESPNOW_GW__


/*
 * ------------------------ Set up the ETHERNET (wired) interface for our gateway ------------------------ 
 */

#define RESET_P	D1              // Tie Wiz820io/W5500 reset pin to NodeMCU pin D1 (GPIO5).
#define TOPIC01	"ESPNow/data"	// MQTT topic for node data.
#define MQ_TOPIC_MAX 256	// MQTT max data buffer size.

/*
 * Define the MAC addresses for our ETHERNET controller.
 *
 * This is our MQTT gateway on the main network.  We need to specify a MAC address
 * here, because the low-cost W5500 boards do -not- have one built in.
 */
uint8_t eth_MAC[] = { 0x02, 0xF0, 0x0D, 0xBE, 0xEF, 0x01 };

/*
 * Define the static network settings for this gateway's ETHERNET connection
 * on your LAN.  These values must match YOUR SPECIFIC LAN.  The "eth_IP"
 * is the IP address for this gateway's ETHERNET port.
 */
IPAddress eth_IP(192, 168, 1, 11);		// CHANGE THIS to something relevant for YOUR LAN.
IPAddress eth_MASK(255, 255, 255, 0);		// Subnet mask.
IPAddress eth_DNS(192, 168, 1, 1);		// DNS server.
IPAddress eth_GW(192, 168, 1, 1);		// Gateway (router).
IPAddress mq_server(192, 168, 1, 100);		// CHANGE THIS to point at YOUR MQTT broker.

EthernetClient ethClient;       // ETHERNET - The gateway wire to the local network.
PubSubClient client(ethClient); // ETHERNET - The MQTT process is attached to the wire, -not- WiFi.

/* ------------------- End of set up for the ETHERNET (wired) interface for our gateway ------------------ */


/* 
 * ------------------------ Set up the WIFI (ESP-Now) interface for our gateway ------------------------- 
 */

String deviceMac;
volatile boolean haveReading = false;
uint8_t rec_MAC[6];

/* ------------------- End of set up for the WIFI (ESP-Now) interface for our gateway ------------------- */



#endif
