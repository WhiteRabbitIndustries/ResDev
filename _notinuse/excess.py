
# setup serial for GPS
"""
ser = serial.Serial(
              
  port='/dev/ttyS0',
  baudrate = 9600,
  parity=serial.PARITY_NONE,
  stopbits=serial.STOPBITS_ONE,
  bytesize=serial.EIGHTBITS,
  timeout=1
  print "serial server set up"
)
"""
# transmit gps settings

#ser.write('$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28') #
#ser.write('$PMTK220,1000*1F')

"""
print "rate setting"
"$PMTK300,1000,0,0,0,0*1C" // Sets rate to 1HZ
"$PMTK300,200,0,0,0,0*2F" // Sets rate to 5HZ
"$PMTK300,100,0,0,0,0*2" // Sets rate to 10HZ
"""

