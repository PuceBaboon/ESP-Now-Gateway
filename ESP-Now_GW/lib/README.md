# WIZ Ethernet Library for Arduino IDE-1.6.4

The Ethernet library lets you connect to the Internet or a local network.

* Supported devices
	* W5500 : ioShield, WIZ550io, W5500 Ethernet Shield, Arduino Ethernet Shield 2
	* W5200 : W5200 Ethernet Shield, WIZ820io
	* W5100 : Arduino Ethernet Shield


* Software
	* Install WIZ Ethernet library IDE-1.6.4
		* Download all files
		* Overwrite "Ethernet" folder onto the "Arduino\libraries\Ethernet" folder in Arduino sketch.

	* Select device(shield)
		* Uncomment device(shiel) you want to use in **$/Ethernet/src/utility/w5100.h**
			```cpp
			//#define W5100_ETHERNET_SHIELD // Arduino Ethenret Shield and Compatibles ...
			//#define W5200_ETHERNET_SHIELD // WIZ820io, W5200 Ethernet Shield 
			#define W5500_ETHERNET_SHIELD   // WIZ550io, ioShield series of WIZnet
			```
        
        * If WIZ550io used, uncommnet "#define WIZ550io_WITH_MACAADDRESS" in **$/Ethernet/src/utility/w5100.h**
			```cpp
			#if defined(W5500_ETHERNET_SHIELD)
			//#define WIZ550io_WITH_MACADDRESS // Use assigned MAC address of WIZ550io
			#include "w5500.h"
			#endif
			```

	* Using the WIZ Ethernet library and evaluate existing Ethernet example.

		All other steps are the same as the steps from the Arduino Ethernet Shield. You can use examples in 	./Ethernet/examples folder for the Arduino IDE 1.6.4, go to Files->Examples->Ethernet, open any example, then copy it to your sketch file and change configuration values properly.
		After that, you can check if it is work well. For example, if you choose 'WebServer', you should change IP Address first and compile and download it. Then you can access web server page through your web browser of your PC or something.

* What is new ?
	* Added new functions
		* sockStatus(SOCKET s) = readSnSR(SOCKET s)
			```cpp
			uint8_t socketStatus(SOCKET s)
			{
				  SPI.beginTransaction(SPI_ETHERNET_SETTINGS);
				  uint8_t status = W5100.readSnSR(s);
				  SPI.endTransaction();
				  return status;
			}
			```

		* reavAvalable(SOCKET s) = getRxReceiveSize(SOCKET s)
			```cpp
			int16_t recvAvailable(SOCKET s)
			{
				  SPI.beginTransaction(SPI_ETHERNET_SETTINGS);
				  int16_t ret = W5100.getRXReceivedSize(s);
				  SPI.endTransaction();
				  return ret;
			}
			```

	* Added SPI Transaction APIs
To solve conflicts that sometimes occur between multiple SPI devices when using SPI from interrupts and/or different SPI settings, SPI Transcation APIs use between between read and write SPI functions.  
        
		```cpp
        SPI.beginTransaction(SPI_ETHERNET_SETTINGS);
        W5100.setIPAddress(_dhcp->getLocalIp().raw_address());
        W5100.setGatewayIp(_dhcp->getGatewayIp().raw_address());
        W5100.setSubnetMask(_dhcp->getSubnetMask().raw_address());
        SPI.endTransaction();
		```
	* Removed Twitter.cpp / Twitter.h


* Revision History  
Initial Release : 21 May. 2015

