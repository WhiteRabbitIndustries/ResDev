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




