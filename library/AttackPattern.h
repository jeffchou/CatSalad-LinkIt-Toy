#ifndef __ATTACK_PATTERN_H__
#define __ATTACK_PATTERN_H__

#include <Arduino.h>

class AttackPattern {
private:
	String id;
	String name;
	String createdAt;
	String updatedAt;
	
	void init(char* json);
	
public:
	AttackPattern(String json);
	AttackPattern(char* json);

	String getId();
	String getName();
	String getCreatedAt();
	String getUpdatedAt();
};

#endif
