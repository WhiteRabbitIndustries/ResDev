#!/usr/bin/env python
#
# Extract BSSID/SSID/Channel from Beacon and Probe Response frames in a capture file
#
# Requires tcpdump to be installed
# tested with: 
# Python 2.7.6
# tcpdump version 4.5.1
# libpcap version 1.5.3
#

import sys, getopt
import subprocess
import re

def usage():
   print "%s -i <capture-file>" % (__file__)

def main(argv):
   try:
      opts, args = getopt.getopt(argv,"hi:",["ifile="])
      if not opts:
         print 'No input file supplied'
         usage()
         sys.exit(2)
   except getopt.GetoptError, e:
      print e
      usage()
      sys.exit(2)
   for opt, arg in opts:
      if opt == '-h':
         usage()
         sys.exit()
      elif opt in ("-i", "--ifile"):
         filename = arg
   return filename

if __name__ == "__main__":
   filename = main(sys.argv[1:])
   list = []
   argv = ["-ennr", filename, "(type mgt subtype beacon) || (type mgt subtype probe-resp)"]
   cmd = subprocess.Popen(["tcpdump"] + argv, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
   print ("BSSID\t-\tSSID\t-\tChannel\n")
   for line in cmd.stdout:
      if 'Beacon' in line:
         bssid = re.search(r'(BSSID:)(\w+\:\w+\:\w+\:\w+\:\w+\:\w+)', line)
         if bssid.group(2) not in list :
            list.append(bssid.group(2))
            ssid = re.search(r'(Beacon\s\()(.+?(?=\)))', line)
            channel = re.search(r'(CH:\s)(\w)', line)
            if ssid:
               print "%s\t%s\t%s" %(bssid.group(2), ssid.group(2), channel.group(2))
            else:
               print "%s\t<hidden>\t%s" %(bssid.group(2), channel.group(2))
      elif 'Probe Response' in line:
         bssid = re.search(r'(BSSID:)(\w+\:\w+\:\w+\:\w+\:\w+\:\w+)', line)
         if bssid.group(2) not in list :
            list.append(bssid.group(2))
            ssid = re.search(r'(Probe Response\s\()(.+?(?=\)))', line)
            channel = re.search(r'(CH:\s)(\w)', line)
            if ssid:
               print "%s\t%s\t%s\t*" %(bssid.group(2), ssid.group(2), channel.group(2))
            else:
                  print "%s\t<hidden>\t%s\t*" %(bssid.group(2), channel.group(2))
   print ("\n* = Probe Response\n")
 