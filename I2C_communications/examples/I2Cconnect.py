import quick2wire.i2c as i2c

address = 0x09
#iodir_register = 0x00
#gpio_register = 0x09

with i2c.I2CMaster(2) as bus:    

    read_results = bus.transaction(
        i2c.reading(address, 6))

    

    print(read_results)

    
try:
	import quick2wire.i2c as i2c
except:
	print("Could not load the quickwire i2c lib, stopping")
	exit(0)

	
######################################## constructor ##################################################
	def __init__(self, bus="", address="", warnings=1):
		if(bus==""):	
			# create connection
			self.bus = i2c.I2CMaster(1)
		else:
			self.bus = bus
		if(address==""):
			self.address = 0x04
		else:
			self.address = address
		self.warnings=warnings
		self.reset_config() # reset controller and vars