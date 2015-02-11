@mainpage

# WeeESP8266

An ESP8266 library for Arduino providing an easy-to-use way to manipulate ESP8266.

## API List

    bool 	kick (void) : Verify ESP8266 whether live or not.
     
    bool 	restart (void) : Restart ESP8266 by "AT+RST".
     
    String 	getVersion (void) : Get the version of AT Command Set.
     
    bool 	setOprToStation (void) : Set operation mode to staion.
     
    bool 	setOprToSoftAP (void) : Set operation mode to softap.
     
    bool 	setOprToStationSoftAP (void) : Set operation mode to station + softap.
     
    String 	getAPList (void) : Search available AP list and return it.
     
    bool 	joinAP (String ssid, String pwd) : Join in AP. 
     
    bool 	leaveAP (void) : Leave AP joined before. 
     
    bool 	setSoftAPParam (String ssid, String pwd, uint8_t chl=7, uint8_t ecn=4) : Set SoftAP parameters. 
     
    String 	getJoinedDeviceIP (void) : Get the IP list of devices connected to SoftAP. 
     
    String 	getIPStatus (void) : Get the current status of connection(UDP and TCP). 
     
    String 	getLocalIP (void) : Get the IP address of ESP8266. 
     
    bool 	enableMUX (void) : Enable IP MUX(multiple connection mode). 
     
    bool 	disableMUX (void) : Disable IP MUX(single connection mode). 
     
    bool 	createTCP (String addr, uint32_t port) : Create TCP connection in single mode. 
     
    bool 	releaseTCP (void) : Release TCP connection in single mode. 
     
    bool 	registerUDP (String addr, uint32_t port) : Register UDP port number in single mode. 
     
    bool 	unregisterUDP (void) : Unregister UDP port number in single mode. 
     
    bool 	createTCP (uint8_t mux_id, String addr, uint32_t port) : Create TCP connection in multiple mode. 
     
    bool 	releaseTCP (uint8_t mux_id) : Release TCP connection in multiple mode. 
     
    bool 	registerUDP (uint8_t mux_id, String addr, uint32_t port) : Register UDP port number in multiple mode. 
     
    bool 	unregisterUDP (uint8_t mux_id) : Unregister UDP port number in multiple mode. 
     
    bool 	setTCPServerTimeout (uint32_t timeout=180) : Set the timeout of TCP Server. 
     
    bool 	startTCPServer (uint32_t port=333) : Start TCP Server(Only in multiple mode). 
     
    bool 	stopTCPServer (void) : Stop TCP Server(Only in multiple mode). 
     
    bool 	send (const uint8_t *buffer, uint32_t len) : Send data based on TCP or UDP builded already in single mode. 
     
    bool 	send (uint8_t mux_id, const uint8_t *buffer, uint32_t len) : Send data based on one of TCP or UDP builded already in multiple mode. 
     
    uint32_t 	recv (uint8_t *buffer, uint32_t buffer_size, uint32_t timeout=1000) : Receive data from TCP or UDP builded already in single mode. 
     
    uint32_t 	recv (uint8_t mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout=1000) : Receive data from one of TCP or UDP builded already in multiple mode. 
     
    uint32_t 	recv (uint8_t *coming_mux_id, uint8_t *buffer, uint32_t buffer_size, uint32_t timeout=1000) : Receive data from all of TCP or UDP builded already in multiple mode. 


# Source 

Source can be download at <https://github.com/itead/ITEADLIB_Arduino_WeeESP8266>.

You can clone it by:

    git clone https://github.com/itead/ITEADLIB_Arduino_WeeESP8266.git

# Documentation

Online API documentation can be reached at <http://docs.iteadstudio.com/ITEADLIB_Arduino_WeeESP8266/index.html>.

Offline API documentation can be found under directory 
[doc](https://github.com/itead/ITEADLIB_Arduino_WeeESP8266/tree/master/doc).

# How to get started

On the home page of API documentation, the tabs of Modules, Classes and Examples 
will be useful for Arduino developers. 

# Mainboard Requires

  - RAM: not less than 2KBytes
  - UART: one hardware serial at least 

# Suppported Mainboards

  - WBoard Pro
  - MEGA and its derivatives

-------------------------------------------------------------------------------

# The End!

-------------------------------------------------------------------------------
