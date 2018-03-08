
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

### Static IP or DHCP?
Preface ...Just to be 100% clear, this section is only referring to the W5500 ethernet connection.  ESP-Now connections don't use IPs or routing and certainly won't use DHCP.

The code as it stands uses a static set-up for the ethernet IP, DNS, netmask and router.  It is very, very easy to change this to DHCP if you prefer to use that method; you only need to change one line in the code.

Open up ESP-Now_GW/src/espnow_gw.ino and locate this block of comments and code inside setup():-

```
   /* 
     * Network configuration - all except the MAC are optional.
     *
     * IMPORTANT NOTE - The mass-produced W5500 boards do -not-
     *                  have a built-in MAC address (depite 
     *                  comments to the contrary elsewhere). You
     *                  -must- supply a MAC address here.
     */
    Ethernet.begin(eth_MAC, eth_IP, eth_DNS, eth_GW, eth_MASK);
```
~~Change the Ethernet.begin call to:-~~

~~Ethernet.begin(eth_MAC);~~

~~...and recompile.  You're done!~~

   **...Okay, forget DHCP for the time being.**  It looks like the ESP8266 has timeout problems while it's waiting for the DHCP process to complete (thanks to user "1rabbit" for bringing that to my attention).  You'll need to assign the W5500 network configuration manually for the time being.
   
