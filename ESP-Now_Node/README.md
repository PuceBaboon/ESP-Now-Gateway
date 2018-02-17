
# ESP-Now_Node
This is the ESP-Now node code, based on Hiroh Satoh's ESP8266-esp-now-sample.

The only real change from the original is the addition of some simple random number code to simulate changing "data" from an attached sensor.

The MAC address of your ESP8266 module **must** be added into the espnow_common.h file in the ESP-Now_GW/src directory to have ESP-Now function reliably.

### espnow_common.h
The src/espnow_common.h file needs to be kept in sync between the gateway and nodes, so (if you're working on some flavour of BSD or Linux) I'd suggest making the version in this ESP-Now_Node directory a symbolic link to the one in the gateway directory and make all updates/change there; I have no idea how you'd do this on other platforms, though.
