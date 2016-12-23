import math
import numpy


class WifiPoint:

   def __init__(self, lat, lon, dist):
      self.lat = lat
      self.lon = lon
      self.dist = dist
  


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

	currentPoint = WifiPoint(lat,lon,0)
	return currentPoint


wifiPoints = []
wifiPoints.append( WifiPoint(37.418436,-121.963477,0.265710701754) )
wifiPoints.append( WifiPoint(37.417243,-121.961889,0.234592423446) )
wifiPoints.append( WifiPoint(37.418692,-121.960194, 0.0548954278262) )


currentPoint = Trilateration(wifiPoints)

print currentPoint.lat, currentPoint.lon, currentPoint.dist