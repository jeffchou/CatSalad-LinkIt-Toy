#ifndef __BATTLE_H__
#define __BATTLE_H__

#include <Arduino.h>

class Battle {
private:
	String id;
	String name;
	String activatedAt;
	String createdAt;
	String updatedAt;
	
	void init(char* json);
	
public:
	Battle(String json);
	Battle(char* json);

	String getId();
	String getName();
	String getActivatedAt();
	String getCreatedAt();
	String getUpdatedAt();
};

#endif
