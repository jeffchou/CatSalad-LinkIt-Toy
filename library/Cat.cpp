
#include "Definition.h"
#include "Cat.h"
#include "EquippedWeapon.h"

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

Cat::Cat(String json)
{
	char jsonCharArray[512];
  json.toCharArray(jsonCharArray, 512);
	
	init(jsonCharArray);
}

Cat::Cat(char* json)
{
	init(json);
}

void Cat::init(char* json)
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
		weaponId = String((char*)root["equipped_weapon_id"]);
		createdAt = String((char*)root["created_at"]);
		updatedAt = String((char*)root["updated_at"]);
		
		HTTP_DEBUG_PRINT("id: ");
		HTTP_DEBUG_PRINTLN(id);
		HTTP_DEBUG_PRINT("name: ");
		HTTP_DEBUG_PRINTLN(name);
		HTTP_DEBUG_PRINT("weapon id: ");
		HTTP_DEBUG_PRINTLN(weaponId);
		HTTP_DEBUG_PRINT("createdAt: ");
		HTTP_DEBUG_PRINTLN(createdAt);
		HTTP_DEBUG_PRINT("updatedAt: ");
		HTTP_DEBUG_PRINTLN(updatedAt);
	}
}

void Cat::update(Cat* cat)
{
	this->id = cat->getId();
	this->name = cat->getName();
	this->weaponId = cat->getWeaponId();
	this->createdAt = cat->getCreatedAt();
	this->updatedAt = cat->getUpdatedAt();
}

String Cat::getId()
{
	return id;
}

String Cat::getName()
{
	return name;
}

String Cat::getWeaponId()
{
	return weaponId;
}

String Cat::getCreatedAt()
{
	return createdAt;
}

String Cat::getUpdatedAt()
{
	return updatedAt;
}
