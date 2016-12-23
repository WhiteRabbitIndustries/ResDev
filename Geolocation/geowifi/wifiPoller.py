#file  -- wifiPoller.py --
import threading
import sys
import subprocess
import time
import math


interface = "wlan0"

class wifiPoller(threading.Thread):

  parsed_cells=[]
  running = True

  # You can add or change the functions to parse the properties of each AP (cell)
  # below. They take one argument, the bunch of text describing one cell in iwlist
  # scan and return a property of that cell.
  def __init__(self):
    threading.Thread.__init__(self)
    #self.parsed_cells=[]
    self.running = True     
  
  def stop(self):
    print "Wifi STOPPED"
    self.running = False

  def run(self):
      
      while self.running: 
        cells=[[]]
        self.parsed_waps={}

        proc = subprocess.Popen(["/sbin/iwlist", interface, "scan"],stdout=subprocess.PIPE, universal_newlines=True)
        out, err = proc.communicate()

        for line in out.split("\n"):
            cell_line = self.match(line,"Cell ")
            if cell_line != None:
                cells.append([])
                line = cell_line[-27:]
            cells[-1].append(line.rstrip())

        cells=cells[1:]
        #print cells

        for cell in cells:
            print "BSIID: " + self.get_name(cell) + "Distance: " + str(self.get_distance(cell)) + "m"
            self.parsed_waps.update({self.get_name(cell):self.get_distance(cell)})
            
        #sort_cells(parsed_cells)

        time.sleep(2) # tune this, you might not get values that quickly


  def get_name(self, cell):
      return self.matching_line(cell,"ESSID:")[1:-1]

  def get_quality(self, cell):
      quality = self.matching_line(cell,"Quality=").split()[0].split('/')
      #return str(int(round(float(quality[0]) / float(quality[1]) * 100))).rjust(3) + " %"
      return int(round(float(quality[0]) / float(quality[1]) * 100))

  def get_address(self, cell):
      return self.matching_line(cell,"Address: ")

  def get_signal_level(self, cell):
      # Signal level is on same line as Quality data so a bit of ugly
      # hacking needed...
      signalString = self.matching_line(cell,"Quality=").split("Signal level=")[1]
      return int(signalString.split(' ')[0])

  def get_frequency(self, cell):
    frequency =  self.matching_line(cell,"Frequency:").split(" ")[0]
    return float(frequency)

  def get_distance(self, cell):
    freq = self.get_frequency(cell)
    freqInMHz = freq*1000
    db = float (self.get_signal_level(cell))
    return self.calculateDistanceInM(db , freqInMHz)

  def calculateDistanceInM(self, levelInDb, freqInMHz):
      exp = (27.55 - (20 * math.log10(freqInMHz)) + math.fabs(levelInDb)) / 20.0
      return pow(10,exp)


  def parse_wap(self,cell):
      """Applies the rules to the bunch of text describing a cell and returns the
      corresponding dictionary"""
      parsed_wap={}
      parsed_cell.update({"Name":self.get_name(cell)})
      parsed_cell.update({"Signal":self.get_signal_level(cell)})
      parsed_cell.update({"Quality":self.get_quality(cell)})
      parsed_cell.update({"Frequency":self.get_quality(cell)})
      parsed_cell.update({"Distance":self.get_quality(cell)})

      #print parsed_cell
      return parsed_cell

  def parse_cell(self,cell):
      """Applies the rules to the bunch of text describing a cell and returns the
      corresponding dictionary"""
      parsed_cell={}
      parsed_cell.update({"Name":self.get_name(cell)})
      parsed_cell.update({"Signal":self.get_signal_level(cell)})
      parsed_cell.update({"Quality":self.get_quality(cell)})
      parsed_cell.update({"Frequency":self.get_quality(cell)})
      parsed_cell.update({"Distance":self.get_quality(cell)})

      #print parsed_cell
      return parsed_cell
      
  def matching_line(self,lines, keyword):
      """Returns the first matching line in a list of lines. See match()"""
      for line in lines:
          matching=self.match(line,keyword)
          if matching!=None:
              return matching
      return None

  def match(self,line,keyword):
      """If the first part of line (modulo blanks) matches keyword,
      returns the end of that line. Otherwise returns None"""
      line=line.lstrip()
      length=len(keyword)
      if line[:length] == keyword:
          return line[length:]
      else:
          return None



  






