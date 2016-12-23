import csv
import sys
import sqlite3
import os.path
import math
import numpy



class WifiPoint:

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

def remove_delimiters (delimiters, s):
    new_s = s
    for i in delimiters: #replace each delimiter in turn with a space
        new_s = new_s.replace(i, '')
    return ' '.join(new_s.split())

def formatBSSID (bssid):
	bssid = remove_delimiters(":",bssid)
	bssid = bssid.upper()
	return bssid

def cropCsvDataBase(csvDbPath, csvDbPathCropped, coordinates):

	csv.register_dialect(
    'mydialect',
    delimiter = '\t',
    quotechar = '"',
    doublequote = True,
    skipinitialspace = True,
    lineterminator = '\r\n',
    quoting = csv.QUOTE_MINIMAL)

    
	if not (os.path.exists(csvDbPathCropped)):
		f = open(csvDbPathCropped, 'w')
		thedatawriter = csv.writer(f, dialect='mydialect')

		print "Cropping " + csvDbPath + " to " + csvDbPathCropped

		with open(csvDbPath, 'rb') as mycsvfile:
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
		print csvDbPathCropped + " file already exit "


def createSQLiteDatabase(csvDbPath, sqliteDbPath):

	csv.register_dialect(
    'mydialect',
    delimiter = '\t',
    quotechar = '"',
    doublequote = True,
    skipinitialspace = True,
    lineterminator = '\r\n',
    quoting = csv.QUOTE_MINIMAL)
	
	conn = sqlite3.connect(sqliteDbPath)
	c = conn.cursor()

	try:
		c.execute('''CREATE TABLE bssids  (bssid text, lat real, lon real)''')

		print "Creating " + sqliteDbPath + " Sqlite data base"
		with open(csvDbPath, 'rb') as mycsvfile:
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
	    print sqliteDbPath + " Sqlite data base ALREADY EXISTS!"


def retrieveWifiPoints(bbsids, sqliteDbPath):

	csv.register_dialect(
    'mydialect',
    delimiter = '\t',
    quotechar = '"',
    doublequote = True,
    skipinitialspace = True,
    lineterminator = '\r\n',
    quoting = csv.QUOTE_MINIMAL)
	
	conn = sqlite3.connect(sqliteDbPath)
	c = conn.cursor()
	wifiPoints = []

	for bssid in bssids:
		t = (bssid,)
		c.execute('SELECT * FROM bssids WHERE bssid=?', t)
		row = c.fetchone()
		print row

		if row != None:	
			wifiPoint = WifiPoint(bssid, row[1], row[2], 0.3)
			wifiPoints.append(wifiPoint)
			print wifiPoint

	return wifiPoints


def Trilateration(points):
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

	currentPoint = WifiPoint("MycurrentPosition", lat,lon,0)
	return currentPoint


coordinates = CroppingCoordinates(40.3176, 40.5493, -3.9015, -3.5088) #Madrid 
#coordinates = CroppingCoordinates(47.9743, 48.4420, 16.1629, 16.5818) #Vienna
#coordinates = CroppingCoordinates(52.3127, 52.7213, 12.8499, 13.9279) #Berlin 

csvDB = "db.csv"
berlinCsvDB = "dbBerlin.csv"
berlinSQliteDB = "dbBerlin.db"
madridCsvDB = "dbMadrid.csv"
madridSQliteDB = "dbMadrid.db"


cropCsvDataBase(csvDB,madridCsvDB, coordinates)
createSQLiteDatabase(madridCsvDB,madridSQliteDB)

bssids = []
bssids.append(formatBSSID("78:9f:70:7a:6d:c0"))
bssids.append("00216315E914")
bssids.append("001A4F24C4F1")
bssids.append("002308905A7B")

wifiPoints = retrieveWifiPoints(bssids, berlinSQliteDB)
currentPoint = Trilateration(wifiPoints)

print currentPoint.bssid, currentPoint.lat, currentPoint.lon


#SELECT sql FROM sqlite_mbaster WHERE type = 'table' AND tbl_name = 'COMPANY';







	







