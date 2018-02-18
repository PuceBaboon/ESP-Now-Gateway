# ESP-Now-Gateway
ESP8266 with W5500 ethernet running as a gateway for ESP-Now nodes.  Node demo code included.

## More information
There's more information on this project available at [ESP8266-Hints](https://esp8266hints.wordpress.com/2018/02/13/adding-an-ethernet-port-to-your-esp-revisited/).

## Components
There are two parts to this repository, the gateway code (ESP-Now_Gateway) and the demo/exerciser code for a standalone sensor node (ESP-Now_Node).

The ESP-Now_Gateway uses a common WizNet W5500 Ethernet board connected to an ESP8266 to provide the wired connection to your LAN, while the WiFi on the ESP8266 accepts ESP-Now connections from a cluster of up to 20 nodes (intended, in my case anyway, to be battery-powered ESP8266 modules with environmental sensors attached).  It's important to note that neither the gateway nor the nodes require a separate access-point, they work together in point-to-point mode using the minimal ESP-Now library.

The gateway code uses WizNet's own Arduino/ESP8266 library for the W5500, Nick O'Leary's PubSubClient library for the MQTT communications with the broker and a somewhat hacked-up version of Anthony Elder's ESP-Now example code.

The node demo/exerciser is a copy of Hiroh Satoh's ESP8266-esp-now-sample code, with the cludgy addition of some random numbers to simulate a changing sensor readout between deep-sleeps.

## Requirements

### Hardware
- An ESP8266 module with a connected ([details here](ttps://esp8266hints.wordpress.com/2018/02/13/adding-an-ethernet-port-to-your-esp-revisited/) W5500 ethernet board, for use as the gateway.
- A working LAN to connect your wired ethernet board.
- A second ESP8266 (no W5500 required for this one) for use as a node.  NOTE:- You do **not** need a functioning access-point (ESP-Now doesn't use the normal WiFi network).

### Libraries
- The Wiznet Ethernet library is already included in the gateway "lib" directory (it has a minor fix included for the fatal compile time error mentioned below in the "Troubleshooting" section).
- You need to add the PubSubClient library to the gateway directory before building.

### Software
- You must have the latest and greatest version of PlatformIO installed (v3.5.1 or better), otherwise your compile may fail with various "espnow" error messages.


## ESP-Now
The basic idea behind using ESP-Now is that, because the protocol is so lightweight, the battery-powered nodes only need to be powered on for a fraction of the time of a normal TCP/IP-connected unit, thus saving significant power. The ESP-Now protocol doesn't use IP addresses, so there's no need for DHCP or for assigning static addresses, no connecting to an access-point, no routing protocol and none of the other overheads (or error handling!) of TCP/IP.  The ESP-Now modules (gateway and nodes) use the MAC address as the unique ID when addressing each other, so connections are very fast and, providing your sensor node acquires the sensor data before attempting to connect to the gateway, the amount of time which the ESP8266 WiFi is active can be significantly less than a second.


## MAC addresses
The basic W5500 modules come without a MAC address assigned, so you need to manually assign one yourself.  See the information on the "X-bit" at the bottom of page 6 and start of page 7 in this document for details on usable "locally administered" MACs:- https://standards.ieee.org/develop/regauth/tut/eui.pdf
The default MAC for the W5500 ethernet board in my code is 02:60:0D:F0:0D:01.  Feel free to change it.
 
Anthony "TornTrousers" Elder has also suggested in his demo code that the WiFi MAC address used by the gateway should also be changed to use a locally administered number, as this would allow the gateway hardware to be swapped out at any time without disrupting the remote nodes (by flashing the gateway firmware onto a new ESP8266 module, it automaticalley gets the gateway MAC address).  This sounds like an excellent suggestion to me, so the default MAC for the ESP-Now WiFi gateway in my code is 02:60:0D:F0:0D:02.  Again, you don't have to use this suggestion or this particular address, so feel free to change it, but be aware that in this case every node **must** have the new address hard coded.

My final note regarding MACs is this.  My initial impression from reading the very limited documentation on ESP-Now was that the gateway accepted connections from anyone and everyone (up to a limit of 20 separate nodes), as long as they were using the ESP-Now protocol and knew the gateway MAC address.  I assumed that the ESP-Now library managed the linked-list of nodes and added to the list whenever a new node was seen.  My own very limited (so far) testing seems to indicate that this is not the case and that we need to manage the linked-list ourselves (otherwise the gateway only seems to "see" about one message in every thirty, or so from "unlisted" nodes).  To that end I'm simply doing a brute-force add to the linked-list of half-a-dozen node addresses at start-up.  You will need to add your nodes' real MAC address to the client list in espnow_common.h to have the node work reliably.


## Troubleshooting

### Compile errors
There are two compile time errors; one is just a warning, the other fatal (but trivial).

The warning is because the ESP8266 limits the MAX_SOCK_NUM (maximum number of concurrent sockets) to 4, while the W5500 can handle 8.  The simplest fix for this would be to limit the W5500 to 4 too, but I'd be interested in hearing any ideas on how to handle this a little more graciously (other than the obvious bulk-edit changing the constant to a different name in one or other of the libraries).  In the meantime, feel free to ignore the warning (unless you have a high number of very verbose nodes).

The fatal error is caused because the ESP8266 compile falls over when it tries to include "avr/pgmspace.h" from the src/Twitter.h file (and fails, of course).  I've already put in a pull request for this change on the WizNet library, but in the meantime I've just left a fixed copy of the library in the project lib directory.

**NOTE FOR PLATFORMIO USERS -** You need to have the latest version of PlatformIO installed (v3.5.1 or greater), otherwise the project will fail to link in the espnow library correctly and you'll end up with a screen-full of error messages (all related to ESP-Now function calls).

