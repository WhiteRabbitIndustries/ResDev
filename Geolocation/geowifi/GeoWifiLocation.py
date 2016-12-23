# import libraries:     
import time
from time import sleep
import os
from wifiPoller import *

def main(args):

	threads = []

	wifip = wifiPoller()
	threads.append(wifip)

	wifip.start()

	while len(threads) > 0:
	    try:
	        # Join all threads using a timeout so it doesn't block
	        # Filter out threads which have been joined or are None
	        threads = [t.join(1000) for t in threads if t is not None and t.isAlive()]
	       	print "run"
	        time.sleep(2)
	    except KeyboardInterrupt:
	        print "Ctrl-c received! Sending kill to threads..."
	        for t in threads:
	            t.running = False


if __name__ == '__main__':
	main(sys.argv)