#ifndef __BOSS_H__
#define __BOSS_H__

#include <Arduino.h>

class Boss {
private:
	String id;
	String name;
	String attackPatternId;
	String createdAt;
	String updatedAt;
	
	void init(char* json);
	
public:
	Boss(String json);
	Boss(char* json);
	
	String getId();
	String getName();
	String getAttackPatternId();
	String getCreatedAt();
	String getUpdatedAt();
};

#endif
