#https://github.com/jrmiddle/pisynth
#https://learn.adafruit.com/adafruit-cap1188-breakout/
import smbus
from Adafruit_CAP1188 import Adafruit_CAP1188
bus = smbus.SMBus(2)
device = 0x29

cap = Adafruit_CAP1188(device, bus)
cap.multitouch_enabled = True

cap.leds_linked = True
cap.write_register(Adafruit_CAP1188.STANDBYCFG, 0x30)
