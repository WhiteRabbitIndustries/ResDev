# Playlight 

'''
Important note: When sending messages on I2c, always wait in between messages, otherwise teensy crashes... 
'''


#file  -- playlight.py --

import threading
import smbus
import time
bus = smbus.SMBus(2)
device = 0x09

cmd = ""
message = ""


class playlight(threading.Thread):

  
	running = True

	def __init__(self):
		threading.Thread.__init__(self)
		self.running = True 


	def run(self):
		try:
		while self.running:

		except StopIteration:
      pass


  def animate ():
    while (notInterrupted)
      #animate

  def POV (img imageFile): #or image location 


def StringToBytes(val):
	retVal = []
	for c in val:
		retVal.append(ord(c))
	return retVal

def sendMessage(device, cmd, message):
	cmd=cmd.upper()
	message = message.upper()
	valCmd = ord(cmd)
	valMessage  = StringToBytes(message)
	print("Message: " + cmd + message + " send to device " + str(device))       
	bus.write_i2c_block_data(device, valCmd, valMessage)
	

  ''' Demo Purposes

import threading
import smbus
import time
bus = smbus.SMBus(2)
device = 0x09

cmd = ""
message = ""

def StringToBytes(val):
	retVal = []
	for c in val:
		retVal.append(ord(c))
	return retVal

def sendMessage(device, cmd, message):
	cmd=cmd.upper()
	message = message.upper()
	valCmd = ord(cmd)
	valMessage  = StringToBytes(message)
	print("Message: " + cmd + message + " send to device " + str(device))       
	bus.write_i2c_block_data(device, valCmd, valMessage)
	



sendMessage(device, "I","6")
sendMessage(device, "A","13")
sendMessage(device, "A","01")
sendMessage(device, "A","21")
sendMessage(device, "A","30")
sendMessage(device, "A","46")
sendMessage(device, "A","60")

sendMessage(device, "R","1")
time.sleep(0.1)
sendMessage(device, "R","2")
time.sleep(0.1)
sendMessage(device, "R","3")
time.sleep(0.1)
sendMessage(device, "R","4")
time.sleep(0.1)
sendMessage(device, "R","5")
time.sleep(0.1)
sendMessage(device, "R","6")
time.sleep(0.1)
sendMessage(device, "R","7")
time.sleep(0.1)
sendMessage(device, "R","8")
time.sleep(0.1)
sendMessage(device, "R","9")
time.sleep(0.1)
sendMessage(device, "R","10")



sendMessage(device, "A","60")

sendMessage(device, "R","60")
sendMessage(device, "A","46")
time.sleep(5)
sendMessage(device, "R","46")
sendMessage(device, "A","37")
time.sleep(5)
sendMessage(device, "R","37")
sendMessage(device, "A","13")
time.sleep(5)
sendMessage(device, "R","13")
sendMessage(device, "A","18")
time.sleep(5)
sendMessage(device, "R","18")

for x in range(0, 24):
	message = str(x)
	sendMessage(device, "R",message)
	time.sleep(0.05)

for x in range(0, 61):
	message = str(x)
	sendMessage(device, "A",message)
	time.sleep(0.1)

for x in range(42, 60):
	message = str(x)
	sendMessage(device, "R",message)


for x in range(0, 61):
	message = str(x)
	sendMessage(device, "A",message)

for x in range(0, 61):
	message = str(x)
	sendMessage(device, "R",message)
	time.sleep(0.05)

for x in range(0, 61):
	message = str(x)
	sendMessage(device, "D",message)
	time.sleep(0.01)



'''


