#ifndef __ATTACK_TYPE_H__
#define __ATTACK_TYPE_H__

#include <Arduino.h>

class AttackType {
private:
	String id;
	String name;
	String createdAt;
	String updatedAt;
	
	void init(char* json);
	
public:
	AttackType();
	AttackType(const char* id, const char* name, const char* createdAt, const char* updatedAt);
	AttackType(String json);
	AttackType(char* json);

	String getId();
	String getName();
	String getCreatedAt();
	String getUpdatedAt();
};

#endif
