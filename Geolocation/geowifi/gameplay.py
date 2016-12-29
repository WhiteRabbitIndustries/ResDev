#!/usr/bin/env python
    
print "Welcome to imUntergrund!"

# import libraries:     
import time
from time import sleep
import pygame
import os
import math
from wifiPoller import *
from gameObjectsClass import *
from beaconPoller import *


def main(args):

  # import gameplay values from files: 
  #import xml for locations, audio files.
  gameData = GameData('gamedata.xml')

  #print "Game Location Count: "
  #print gameData.getLocationCount()

  pygame.mixer.init()
  # import state/level (so that)

  # enabling to start listening. can be powered off when not needed, but should check warmstartup conditiosn



  threads = []

  wifip = wifiPoller()
  threads.append(wifip)
  beaconp = beaconPoller()
  threads.append(beaconp)

  for t in threads:
    t.daemon = True
    t.start()

  # main loop
  print "entering main loop"

  try:
    
    while True:
        print "Run"

        currentCoord = wifip.getCurrentLocation()

        print ""
        print currentCoord

        #state machine
        #go through all states. 
        for x in xrange(0,gameData.getLocationCount()-1):
          #self.locationCount = 0
          # if active, check wifi. 
          distance = 0.0

          if gameData.locationList[x].active == True:

            #calc distance based on wifi
            distance = float(math.sqrt((gameData.locationList[x].coord['x'] - currentCoord['x'])**2 + (gameData.locationList[x].coord['y'] - currentCoord['y'])**2))
            distance = distance / 0.00001; #translate to meters
            print "Distance to %s is %d." % (gameData.locationList[x].name, distance)


            inBoundBeacon = False

            # get wifi aps for the location, then check if they are in bound
            for a in xrange(0,len(gameData.locationList[x].beacons)):
              #print "in scan", a

              inBoundBeacon = inBoundBeacon or beaconp.inBeaconRange(gameData.locationList[x].beacons[a]['uuid'],gameData.locationList[x].beacons[a]['rssi'])
              
            print inBoundBeacon


            if (distance<gameData.locationList[x].radius) | inBoundBeacon:
              # location check in: enter initial state, update prereq of other locaitons/states
              print "onlocation"
              #print "wifi strength: %d" % signalStrength
              gameData.locationList[x].onloc = True
            else: 
              gameData.locationList[x].onloc = False

            # if state is active, and if onlocation,
            if gameData.locationList[x].onloc == True:
                # play the current state      
                if gameData.locationList[x].stateList[gameData.locationList[x].currentState].activated == False:
                    #unlocklist
                    print "unlocking locations with id: "
                    unlocklist = gameData.locationList[x].stateList[gameData.locationList[x].currentState].unlocklist
                    print unlocklist
                    for u in range(0,len(unlocklist)):
                        gameData.locationList[unlocklist[u]-1].active = True
                        #print unlocklist[u]
                    
                    gameData.locationList[x].stateList[gameData.locationList[x].currentState].activated = True
          
                #play audio
                if pygame.mixer.music.get_busy() != True:
                    pygame.mixer.music.load(gameData.locationList[x].stateList[gameData.locationList[x].currentState].audiofile)
                    pygame.mixer.music.play()


         


        time.sleep(1)
  
  except KeyboardInterrupt:
      print 'Game End!'


if __name__ == '__main__':
  main(sys.argv)

