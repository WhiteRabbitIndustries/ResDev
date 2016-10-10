#!/usr/bin/python
 
### We import the scapy module into our program here
from scapy.all import *
 
#we define a list calles aps to store all APs we detect
aps = []
 
#The following function is a packet handler that will check each packet as it              
#is passed by the sniffer. If the packet has an 802.11 layer and the type is 0
#which is a management frame and subtype 0. If the AP's address is not already in
# the aps list then add it to the list and print it.
def PacketHandler(pkt) :
        if pkt.haslayer(Dot11) :
                if pkt.type == 0 and pkt.subtype == 8 :
                        if pkt.addr2 not in aps :
                                aps.append(pkt.addr2)
                                print "Found BSSID %s and SSID %s " %(pkt.addr2, pkt.info)
 
#Begin sniffing and pass each packet to the PacketHandler function above.
sniff(iface="mon0", prn = PacketHandler)