#ifndef __TOY_CONTROL_H__
#define __TOY_CONTROL_H__

#include <Arduino.h>

class ToyControl {
private:
	String id;
	String battleId;
	String toyDeviceId;
	String type;
	
	void init(char* json);
	
public:
	ToyControl(String json);
	ToyControl(char* json);
	
	String getId();
	String getBattleId();
	String getToyDeviceId();
	String getType();
};

#endif
