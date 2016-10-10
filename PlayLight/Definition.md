## Playlight

Hexagonal programmable led field. 

LEDs: APA102 or SK9822 (high speed, SDA & SCL, 5V but also works with 3.3V)


Library? FastLED is for Arduino. Must find a good LED managing library for CHIP based on python. 



Functions? 
- aniamtion
- POV
	Must access accelerometer to sync. 
	Also should handle image files (to binary? directly? how to handle bit oeprations? pixel to RGB conversions)
		Probably would need a good iamge handling library/
- Mapping (from location on map to pixel; might also be enough to just put in pixel number on gameData)
- LightEffects, can be itself a custom library (heartbeats, sleep rhythm, flame flicker)
- PixelManager (set status, active, passive, update)
- Status handler (for example, from aniamtion to different status. switching between different effects)
