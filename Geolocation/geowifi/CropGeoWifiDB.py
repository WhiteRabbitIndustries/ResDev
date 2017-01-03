import csv
import sys
import sqlite3
import os.path
  

class CroppingCoordinates:

   def __init__(self, lat1, lat2, lon1, lon2):
      self.lat1 = lat1
      self.lat2 = lat2
      self.lon1 = lon1
      self.lon2 = lon2




#coordinates = CroppingCoordinates(43.2463, 43.2721, -2.9613, -2.9078) #Bilbao 
#coordinates = CroppingCoordinates(40.3176, 40.5493, -3.9015, -3.5088) #Madrid 
coordinates = CroppingCoordinates(47.9743, 48.4420, 16.1629, 16.5818) #Vienna
#coordinates = CroppingCoordinates(52.3127, 52.7213, 12.8499, 13.9279) #Berlin 




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


	if not (os.path.exists(csvDbPath)):
		print csvDbPath + " file does not exit "
		return


	try:
	    os.remove(csvDbPathCropped)
	except OSError:
	    pass


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


def createSQLiteDatabase(csvDbPath, sqliteDbPath):

	csv.register_dialect(
    'mydialect',
    delimiter = '\t',
    quotechar = '"',
    doublequote = True,
    skipinitialspace = True,
    lineterminator = '\r\n',
    quoting = csv.QUOTE_MINIMAL)


	if not (os.path.exists(csvDbPath)):
   		print csvDbPath + " file does not exit "
   		return

	try:
	    os.remove(sqliteDbPath)
	except OSError:
	    pass


	conn = sqlite3.connect(sqliteDbPath)
	c = conn.cursor()
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



csvDB = "db.csv"
csvWaps = "waps.csv"
dbWaps = "waps.db"

cropCsvDataBase(csvDB,csvWaps, coordinates)
createSQLiteDatabase(csvWaps,dbWaps)







	







