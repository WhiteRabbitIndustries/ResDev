## N Degrees of Freedom

The class to handle accelerometer values. May need to be passed to the playlight class, as it directly uses it. 

Future: Gesture class (like shake to get a riddle)



Choice of accelerometer? 
- MPU9250 breakout
	https://electronics.semaf.at/9-Degrees-of-Freedom-MPU9250
	https://github.com/kriswiner/MPU-9250 (only Arduino code)
- miniIMU9
	http://physicalcomputing.at/MinIMU-9
	https://www.pololu.com/product/1268
	


Functions:
- return shake speed (synch for playlight)
- ifShaken
