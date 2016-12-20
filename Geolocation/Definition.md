## Geolocation

Merging library that handles all three ways of locating: GPS, WIFI, BLE

Contains objects of each sub class, and uses them to get handling.

Functions:
isOnLocation(x,y)
isOnLocation([BSSID],[SNR])
isOnLocation(BLE_ID)


isOnRadius()


## BLE based location recognition

Depending on the proximity of the beacon, trigger actions.


Questions:

Install libraries?



## GPS based recognition and positioning


HW: Adafruit Ultimate GPS
https://www.adafruit.com/product/746


Required libraries?
- python GPS?

Issues:
- CHIP uart problem. GPS board works with UART (rx/tx). On CHIP, UART is used for internal broadcasts of UBOOT and kernel messeages. 
	Possibilities: 	- find a way to disable UBOOT and kernel messeages
					- use GPIO for software UART
					- use GPIO for enable / disabling GPS board through EN pin. For this to work, GPIO pin must be preset to LOW on boot (currently HIGH). 


Functions:
- Request Time & Date. 
- Return coordinates.
- isInRadius(float radius, targetX, targetY)


## WIFI based recognition and positioning

Required libraries?


Functions:
- Set Scan frequency (1 second)

METHOD 1: Scan WIFI APs, and query for predefined BSSID and SNR. (WORKING)
	Extension: Use geofencing to store 4 points around game loc.

	-	isOnLocation (BSSID,SNR)
			return true/false


METHOD 2: Use offline API / service to feed in [BSSID] & [SNR], which returns Coordinates (x,y). 
	https://en.wikipedia.org/wiki/Wi-Fi_positioning_system
	Issues: Might be energy & processing power consuming. Difficult to implement. REquires database management.  
	Examples: 	- Wigle https://wigle.net/
				- www.mylnikov.org/
				- geomena http://www.programmableweb.com/api/geomena
				- radiocells https://radiocells.org/
				- Openwifisu https://openwifi.su/
				- Skylift https://github.com/adamhrv/SKYLIFT-research

	- getPosition ([BSSID],[SNR])
		return (x,y)





