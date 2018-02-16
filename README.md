# ESP-Now-Gateway
ESP8266 with W5500 ethernet running as a gateway for ESP-Now nodes.  Node demo code included.

## Components
There are two parts to this repository, the gateway code (ESP-Now_Gateway) and the demo/exerciser code for a standalone sensor node (ESP-Now_Node).

The ESP-Now_Gateway uses a common WizNet W5500 Ethernet board connected to an ESP8266 to provide the wired connection to your LAN, while the WiFi on the ESP8266 accepts ESP-Now connections from a cluster of up to 20 nodes (intended, in my case anyway, to be battery-powered ESP8266 modules with environmental sensors attached).  It's important to note that neither the gateway nor the nodes require a separate access-point, they work together in point-to-point mode using the minimal ESP-Now library.

The gateway code uses WizNet's own Arduino/ESP8266 library for the W5500, Nick O'Leary's PubSubClient library for the MQTT communications with the broker and a somewhat hacked-up version of Anthony Elder's ESP-Now example code.

The node demo/exerciser is a copy of Hiroh Satoh's ESP8266-esp-now-sample code, with the cludgy addition of some random numbers to simulate a changing sensor readout between deep-sleeps.

## ESP-Now
The basic idea behind using ESP-Now is that, because the protocol is so lightweight, the battery-powered nodes only need to be powered on for a fraction of the time of a normal TCP/IP-connected unit, thus saving significant power. The ESP-Now protocol doesn't use IP addresses, so there's no need for DHCP or for assigning static addresses, no connecting to an access-point, no routing protocol and none of the other overheads (or error handling!) of TCP/IP.  The ESP-Now modules (gateway and nodes) use the MAC address as the unique ID when addressing each other, so connections are very fast and, providing your sensor node acquires the sensor data before attempting to connect to the gateway, the amount of time which the ESP8266 WiFi is active can be significantly less than a second.


## Troubleshooting

### Compile errors
There are two compile time errors; one is just a warning, the other fatal (but trivial).

The warning is because the ESP8266 limits the MAX_SOCK_NUM (maximum number of concurrent sockets) to 4, while the W5500 can handle 8.  The simplest fix for this would be to limit the W5500 to 4 too, but I'd be interested in hearing any ideas on how to handle this a little more graciously (other than the obvious bulk-edit changing the constant to a different name in one or other of the libraries).  In the meantime, feel free to ignore the warning (unless you have a high number of very verbose nodes).

The fatal error is caused because the ESP8266 compile falls over when it tries to include the "avr/pgmspace.h" file (and fails, of course).  I've already put in a pull request for this change on the WizNet library, but in the meantime I've just left a fixed copy of the library in the project lib directory.
