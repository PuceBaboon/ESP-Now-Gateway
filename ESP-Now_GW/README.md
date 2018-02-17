
# ESP-Now_GW (Gateway)

The MAC address of your ESP8266 node modules **must** be added into the espnow_common.h file in the ESP-Now_GW/src directory to have ESP-Now function reliably.

### espnow_common.h
The src/espnow_common.h file needs to be kept in sync between the gateway and nodes, so (if you're working on some flavour of BSD or Linux) I'd suggest making the version in the ESP-Now_Node directory a symbolic link to the one in this gateway directory and make all updates/change here.

### Making the W5500 work
For anyone looking for information on the integration of thw W5500 board and the ESP8266, it's worth noting that the magic sauce for this implementation is contained in two lines in the src/espnow_gw.h file:-

```C++
EthernetClient ethClient;       // ETHERNET - The gateway wire to the local network.
PubSubClient client(ethClient); // ETHERNET - The MQTT process is attached to the wire, -not- WiFi.
```

