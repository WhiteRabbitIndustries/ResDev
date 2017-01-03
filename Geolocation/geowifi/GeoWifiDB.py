import csv
import sys
import sqlite3
import os.path
import math
import numpy
import localization as lx



class WirelessAccessPoint:

   def __init__(self, bssid, lat, lon, dist):
   		self.bssid = bssid
   		self.lat = lat
   		self.lon = lon
   		self.dist = dist
  

class CroppingCoordinates:

   def __init__(self, lat1, lat2, lon1, lon2):
      self.lat1 = lat1
      self.lat2 = lat2
      self.lon1 = lon1
      self.lon2 = lon2


class GeoWifiDB:

	def __init__(self):
		self.initAttrtibutes()
		self.loadDB()
		
		
	def initAttrtibutes(self):
		self.csvWaps = "waps.csv"
		self.dbWaps = "waps.db"
	
		self.currentPosition = WirelessAccessPoint("MycurrentPosition", 0,0,0)

	def loadDB(self):

		self.createSQLiteDatabase()


	def remove_delimiters (self, delimiters, s):
	    new_s = s
	    for i in delimiters: #replace each delimiter in turn with a space
	        new_s = new_s.replace(i, '')
	    return ' '.join(new_s.split())

	def formatBSSID (self, bssid):
		bssid = self.remove_delimiters(":",bssid)
		bssid = bssid.upper()
		return bssid

	def createSQLiteDatabase(self):

		csv.register_dialect(
	    'mydialect',
	    delimiter = '\t',
	    quotechar = '"',
	    doublequote = True,
	    skipinitialspace = True,
	    lineterminator = '\r\n',
	    quoting = csv.QUOTE_MINIMAL)
		
		conn = sqlite3.connect(self.dbWaps)
		c = conn.cursor()

		try:
			c.execute('''CREATE TABLE bssids  (bssid text, lat real, lon real)''')

			print "Creating " + self.dbWaps + " Sqlite data base"
			with open(self.csvWaps, 'rb') as mycsvfile:
			    datareader = csv.reader(mycsvfile, dialect='mydialect')
			    #datareader.next() # skips the first (header) row

			    for idx, row in enumerate(datareader):

			    	if idx == 0:
			    		print(row[0]+"\t \t"+row[1]+"\t \t"+row[2])
			        else:

			    		c.execute("INSERT INTO bssids VALUES ('{}','{}','{}')".format(row[0],row[1],row[2]))
			        	if (idx % 100000 == 0):
			    			conn.commit()

		
			sql_index= """CREATE UNIQUE INDEX bssid_index on bssids (bssid);"""
			c.execute(sql_index)
			conn.close()
		except: 
		    print self.dbWaps  + " Sqlite data base ALREADY EXISTS!"


	def retrieveWAPs(self, bssids):

		csv.register_dialect(
	    'mydialect',
	    delimiter = '\t',
	    quotechar = '"',
	    doublequote = True,
	    skipinitialspace = True,
	    lineterminator = '\r\n',
	    quoting = csv.QUOTE_MINIMAL)
		
		conn = sqlite3.connect(self.dbWaps)
		c = conn.cursor()
		waps = []

		## This loop syntax accesses the whole dict
  		for bssid, distance in bssids.items(): 
  			t = (self.formatBSSID(bssid),)
  			c.execute('SELECT * FROM bssids WHERE bssid=?', t)
			row = c.fetchone()
			#print row

			if row != None:	
				wap = WirelessAccessPoint(row[0], row[1], row[2], distance/1000)
				waps.append(wap)
				print "WAP -> bssid: " + wap.bssid + ", lat: " + str(wap.lat) + ", lon: " + str(wap.lon) + ", distance: " + str(wap.dist)

		print "RetrieveWAPs -> Number of WAPS in Data Base: " + str(len(waps))
		return waps


	def getCurrentLocation(self, bssids):
		waps = self.retrieveWAPs(bssids)
		return self.multilateration(waps)

	def multilateration(self, waps):

		if len(waps) < 3:
			print "Multilateration:: Not enough data points -> " + str(len(waps))
			return self.currentPosition

		P=lx.Project(mode="Earth1",solver="LSE")

		for wap in waps:
			P.add_anchor(wap.bssid,(wap.lat, wap.lon))

		t,label=P.add_target()

		for wap in waps:
			t.add_measure(wap.bssid,wap.dist)

		P.solve()

		self.currentPosition.lat = t.loc.x
		self.currentPosition.lon = t.loc.y
		return self.currentPosition



	def trilateration(self, points):

		if len(points) < 3:
			print "Trilateration:: Not enough data points -> " + str(len(points))
			return self.currentPosition


		# assuming elevation = 0
		# length unit : km
		earthR = 6371

		pointA = points[0]
		pointB = points[1]
		pointC = points[2]

		print "pointA -> bssid: " + pointA.bssid + ", lat: " + str(pointA.lat) + ", lon: " + str(pointA.lon) + ", distance: " + str(pointA.dist)
		print "pointB -> bssid: " + pointB.bssid + ", lat: " + str(pointB.lat) + ", lon: " + str(pointB.lon) + ", distance: " + str(pointB.dist)
		print "pointC -> bssid: " + pointC.bssid + ", lat: " + str(pointC.lat) + ", lon: " + str(pointC.lon) + ", distance: " + str(pointC.dist)

		#using authalic sphere
		#if using an ellipsoid this step is slightly different
		#Convert geodetic Lat/Long to ECEF xyz
		#   1. Convert Lat/Long to radians
		#   2. Convert Lat/Long(radians) to ECEF
		xA = earthR *(math.cos(math.radians(pointA.lat)) * math.cos(math.radians(pointA.lon)))
		yA = earthR *(math.cos(math.radians(pointA.lat)) * math.sin(math.radians(pointA.lon)))
		zA = earthR *(math.sin(math.radians(pointA.lat)))

		xB = earthR *(math.cos(math.radians(pointB.lat)) * math.cos(math.radians(pointB.lon)))
		yB = earthR *(math.cos(math.radians(pointB.lat)) * math.sin(math.radians(pointB.lon)))
		zB = earthR *(math.sin(math.radians(pointB.lat)))

		xC = earthR *(math.cos(math.radians(pointC.lat)) * math.cos(math.radians(pointC.lon)))
		yC = earthR *(math.cos(math.radians(pointC.lat)) * math.sin(math.radians(pointC.lon)))
		zC = earthR *(math.sin(math.radians(pointC.lat)))

		P1 = numpy.array([xA, yA, zA])
		P2 = numpy.array([xB, yB, zB])
		P3 = numpy.array([xC, yC, zC])

		#from wikipedia
		#transform to get circle 1 at origin
		#transform to get circle 2 on x axis
		ex = (P2 - P1)/(numpy.linalg.norm(P2 - P1))
		i = numpy.dot(ex, P3 - P1)
		ey = (P3 - P1 - i*ex)/(numpy.linalg.norm(P3 - P1 - i*ex))
		ez = numpy.cross(ex,ey)
		d = numpy.linalg.norm(P2 - P1)
		j = numpy.dot(ey, P3 - P1)

		#from wikipedia
		#plug and chug using above values
		x = (pow(pointA.dist,2) - pow(pointB.dist,2) + pow(d,2))/(2*d)
		y = ((pow(pointA.dist,2) - pow(pointC.dist,2) + pow(i,2) + pow(j,2))/(2*j)) - ((i/j)*x)

		# only one case shown here
		z = numpy.sqrt(pow(pointA.dist,2) - pow(x,2) - pow(y,2))

		#triPt is an array with ECEF x,y,z of trilateration point
		triPt = P1 + x*ex + y*ey + z*ez

		#convert back to lat/long from ECEF
		#convert to degrees
		lat = math.degrees(math.asin(triPt[2] / earthR))
		lon = math.degrees(math.atan2(triPt[1],triPt[0]))

		self.currentPosition.lat = lat
		self.currentPosition.lon = lon
		return self.currentPosition



geoWifiDb = GeoWifiDB()

# bssids = {}
# bssids.update({"001BB103CC88": 0.2})
# bssids.update({"C2CE46D44285": 0.2})
# bssids.update({"C8D71988DD01": 0.2})

# # wifiPoints = geoWifiDb.retrieveWAPs(bssids)
# # currentPoint = geoWifiDb.trilateration(wifiPoints)

# currentPoint = geoWifiDb.getCurrentLocation(bssids)

# print currentPoint.bssid, currentPoint.lat, currentPoint.lon








	







