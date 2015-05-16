#ifndef __EQUIPPED_WEAPON_H__
#define __EQUIPPED_WEAPON_H__

#include <Arduino.h>

class EquippedWeapon {
private:
	String id;
	String name;
	double attackBonus;
	String createdAt;
	String updatedAt;
	
	void init(char* json);
	
public:
	EquippedWeapon(String json);
	EquippedWeapon(char* json);

	String getId();
	String getName();
	double getAttackBonus();
	String getCreatedAt();
	String getUpdatedAt();
};

#endif
