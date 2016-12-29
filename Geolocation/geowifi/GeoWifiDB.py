import csv
import sys
import sqlite3
import os.path
import math
import numpy



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
		self.csvDB = "wifi.csv"
		self.croppedCsvDB = "dbWifiBilbao.csv"
		self.sqliteDB = "dbWifiBilbao.db"
		self.coordinates = CroppingCoordinates(40.3176, 40.5493, -3.9015, -3.5088) #Madrid 
		#coordinates = CroppingCoordinates(47.9743, 48.4420, 16.1629, 16.5818) #Vienna
		#coordinates = CroppingCoordinates(52.3127, 52.7213, 12.8499, 13.9279) #Berlin 
		#self.croppedCsvDB = "dbBerlin.csv"
		#self.sqliteDB = "dbBerlin.db" 
		self.currentPosition = WirelessAccessPoint("MycurrentPosition", 0,0,0)

	def loadDB(self):

		self.cropCsvDataBase()
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

	def cropCsvDataBase(self):

		csv.register_dialect(
	    'mydialect',
	    delimiter = '\t',
	    quotechar = '"',
	    doublequote = True,
	    skipinitialspace = True,
	    lineterminator = '\r\n',
	    quoting = csv.QUOTE_MINIMAL)


		print "Cropping DB to " + self.croppedCsvDB

		if not (os.path.exists(self.croppedCsvDB)):
			f = open(self.croppedCsvDB, 'w')
			thedatawriter = csv.writer(f, dialect='mydialect')

			print "Cropping " + self.csvDB + " to " + self.croppedCsvDB

			with open(self.csvDB , 'rb') as mycsvfile:
			    datareader = csv.reader(mycsvfile, dialect='mydialect')
			    #datareader.next() # skips the first (header) row

			    for idx, row in enumerate(datareader):

			    	if idx == 0:
			    		print(row[0]+"\t \t"+row[1]+"\t \t"+row[2])
			        	thedatawriter.writerow(row)
			        else:

			    		if coordinates.lat1 < float(row[1]) and coordinates.lat2 > float(row[1]) and coordinates.lon1 < float(row[2]) and coordinates.lon2 > float(row[2]):
			        		print(row[0]+"\t \t"+row[1]+"\t \t"+row[2])
			        		thedatawriter.writerow(row)
			      

			f.close()
		
		else:
			print self.croppedCsvDB + " file already exit "


	def createSQLiteDatabase(self):

		csv.register_dialect(
	    'mydialect',
	    delimiter = '\t',
	    quotechar = '"',
	    doublequote = True,
	    skipinitialspace = True,
	    lineterminator = '\r\n',
	    quoting = csv.QUOTE_MINIMAL)
		
		conn = sqlite3.connect(self.sqliteDB)
		c = conn.cursor()

		try:
			c.execute('''CREATE TABLE bssids  (bssid text, lat real, lon real)''')

			print "Creating " + self.sqliteDB + " Sqlite data base"
			with open(self.croppedCsvDB, 'rb') as mycsvfile:
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
		    print self.sqliteDB  + " Sqlite data base ALREADY EXISTS!"


	def retrieveWAPs(self, bssids):

		csv.register_dialect(
	    'mydialect',
	    delimiter = '\t',
	    quotechar = '"',
	    doublequote = True,
	    skipinitialspace = True,
	    lineterminator = '\r\n',
	    quoting = csv.QUOTE_MINIMAL)
		
		conn = sqlite3.connect(self.sqliteDB)
		c = conn.cursor()
		waps = []

		## This loop syntax accesses the whole dict
  		for bssid, distance in bssids.items(): 
  			t = (self.formatBSSID(bssid),)
  			c.execute('SELECT * FROM bssids WHERE bssid=?', t)
			row = c.fetchone()
			#print row

			if row != None:	
				wap = WirelessAccessPoint(row[0], row[1], row[2], distance)
				waps.append(wap)
				print "WAP -> bssid: " + wap.bssid + ", lat: " + str(wap.lat) + ", lon: " + str(wap.lon) + ", distance: " + str(wap.dist)

		print "RetrieveWAPs -> Number of WAPS in Data Base: " + str(len(waps))
		return waps


	def getCurrentLocation(self, bssids):
		wifiPoints = self.retrieveWAPs(bssids)
		return self.trilateration(wifiPoints)

	def trilateration(self, points):

		if len(points) < 3:
			print "Trilateration:: Not enough data points -> " + str(len(points))
			return self.currentPosition


		#assuming elevation = 0
		earthR = 6371

		pointA = points[0]
		pointB = points[1]
		pointC = points[2]

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








	







