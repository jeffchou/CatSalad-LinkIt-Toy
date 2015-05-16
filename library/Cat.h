#ifndef __CAT_H__
#define __CAT_H__

#include <Arduino.h>

class Cat {
private:
	String id;
	String name;
	String weaponId;
	String createdAt;
	String updatedAt;
	
	void init(char* json);
	
public:
	Cat(String json);
	Cat(char* json);
	
	void update(Cat* cat);
	
	String getId();
	String getName();
	String getWeaponId();
	String getCreatedAt();
	String getUpdatedAt();
};

#endif
