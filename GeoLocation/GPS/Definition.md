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
