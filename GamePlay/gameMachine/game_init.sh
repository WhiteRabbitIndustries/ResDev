#!/bin/sh
screen -dmS iU ./gamestart.sh 

#!/bin/sh
sudo stty -F /dev/ttyS0 -echo ispeed 9600 line 0 
sudo gpsd /dev/ttyS0 -F /var/run/gpsd.sock&
# wait

#screen game play
sudo screen -dm python /usr/share/adafruit/webide/repositories/ResDev/imUntergrund/gameplay.py