
# ESP-Now_GW (Gateway)

The MAC address of your ESP8266 node modules **must** be added into the espnow_common.h file in the ESP-Now_GW/src directory to have ESP-Now function reliably.

### espnow_common.h
The src/espnow_common.h file needs to be kept in sync between the gateway and nodes, so (if you're working on some flavour of BSD or Linux) I'd suggest making the version in the ESP-Now_Node directory a symbolic link to the one in this gateway directory and make all updates/change here.
