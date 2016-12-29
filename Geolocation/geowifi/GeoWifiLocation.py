# import libraries:     
import time
from time import sleep
import os
from wifiPoller import *

def main(args):

	threads = []

	wifip = wifiPoller()
	threads.append(wifip)

	for t in threads:
		t.daemon = True
		t.start()


	while True:
		print "Run"
		time.sleep(2)


if __name__ == '__main__':
	main(sys.argv)