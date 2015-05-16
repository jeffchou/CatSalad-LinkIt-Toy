
#include "Definition.h"
#include "EquippedWeapon.h"

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

EquippedWeapon::EquippedWeapon(String json)
{
	char jsonCharArray[512];
  json.toCharArray(jsonCharArray, 512);
	
	init(jsonCharArray);
}

EquippedWeapon::EquippedWeapon(char* json)
{
	init(json);
}

void EquippedWeapon::init(char* json)
{
	JsonParser<32> parser;
  JsonObject root = parser.parse(json);

  if (!root.success()) {
    HTTP_DEBUG_PRINTLN("JsonParser.parse() failed");
    return;
  }
	else {
		id = String((char*)root["id"]);
		name = String((char*)root["name"]);
		attackBonus = root["attack_bonus"];
		createdAt = String((char*)root["created_at"]);
		updatedAt = String((char*)root["updated_at"]);
		
		HTTP_DEBUG_PRINT("id: ");
		HTTP_DEBUG_PRINTLN(id);
		HTTP_DEBUG_PRINT("name: ");
		HTTP_DEBUG_PRINTLN(name);
		HTTP_DEBUG_PRINT("attackBonus: ");
		HTTP_DEBUG_PRINTLN(attackBonus);
		HTTP_DEBUG_PRINT("createdAt: ");
		HTTP_DEBUG_PRINTLN(createdAt);
		HTTP_DEBUG_PRINT("updatedAt: ");
		HTTP_DEBUG_PRINTLN(updatedAt);
	}
}

String EquippedWeapon::getId()
{
	return id;
}

String EquippedWeapon::getName()
{
	return name;
}

double EquippedWeapon::getAttackBonus()
{
	return attackBonus;
}

String EquippedWeapon::getCreatedAt()
{
	return createdAt;
}

String EquippedWeapon::getUpdatedAt()
{
	return updatedAt;
}
