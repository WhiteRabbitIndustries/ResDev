#file  -- gpsClass.py --

import gpsClass
import bleClass
import wifiClass

class GeoLoc(threading.Thread):

  def __init__(self):


  def run(self):
    try:
      while self.running:
        self.current_value = self.session.next()
        time.sleep(1) # tune this, you might not get values that quickly
    except StopIteration:
      pass

