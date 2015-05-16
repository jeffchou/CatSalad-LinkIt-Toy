#ifndef __ToyDevice_H__
#define __ToyDevice_H__

#include <Arduino.h>

class ToyDevice {
private:
	String id;
	bool enableSound;
	bool useSystemRecommendedTime;
	String systemRecommendedTime;
	String userScheduledTime;
	double location[2];
	
	void init(char* json);
	
public:
	ToyDevice(String json);
	ToyDevice(char* json);
	
	String getId();
	bool isEnableSound();
	bool isUseSystemRecommendedTime();
	String getSystemRecommendedTime();
	String getUserScheduledTime();
	double* getLocation();
};

#endif
