/*
 *   $Id: espnow_common.h,v 1.2 2018/02/17 00:33:07 gaijin Exp $
 *
 * Defines common to both gateway and nodes.  This should 
 * be copied verbatim to the node src directory (or linked).
 *
 */
#ifndef __ESPNOW_COMMON__
#define __ESPNOW_COMMON__

#define WIFI_DEFAULT_CHANNEL 11

/*
 * Define the MAC address for our -Gateway- WiFi interface.
 * 
 * The ESP-Now nodes use -this- address (not an IP address) to communicate
 * with the gateway, so it needs to be a constant, even if the hardware of
 * the gateway changes.
 */
uint8_t wifi_mac[] = { 0x02, 0x60, 0x0D, 0xF0, 0x0D, 0x02 };


/*
 * Define our remote "Controller" device MACs here.
 *
 * YOU MUST ADD your node MAC addresses here (up to 20 of
 * them), to have ESP-Now function reliably.
 */
uint8_t cont01_mac[] = { 0x18, 0xFE, 0x34, 0xD4, 0x2E, 0x51 };	// Yellow Dev test node.
uint8_t cont02_mac[] = { 0x02, 0xFE, 0xED, 0xBE, 0xEF, 0x04 };	// Change me!
uint8_t cont03_mac[] = { 0x02, 0xFE, 0xED, 0xBE, 0xEF, 0x05 };	// Change me!
uint8_t cont04_mac[] = { 0x02, 0xFE, 0xED, 0xBE, 0xEF, 0x06 };	// Change me!
uint8_t cont05_mac[] = { 0x02, 0xFE, 0xED, 0xBE, 0xEF, 0x07 };	// Change me!
uint8_t cont06_mac[] = { 0x02, 0xFE, 0xED, 0xBE, 0xEF, 0x08 };	// Change me!

/*
 * The gateway and nodes must share an identical data structure.
 */
struct __attribute__ ((packed)) SENSOR_DATA {
    uint8_t	loc_id;
    float	temp;
    float	humidity;
    float	pressure;
} sensorData;


#endif
