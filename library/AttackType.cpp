
#include "Definition.h"
#include "AttackType.h"

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

AttackType::AttackType()
{
}

AttackType::AttackType(const char* id, const char* name, const char* createdAt, const char* updatedAt)
{
	this->id = String(id);
	this->name = String(name);
	this->createdAt = String(createdAt);
	this->updatedAt = String(updatedAt);
}

AttackType::AttackType(String json)
{
	char jsonCharArray[512];
  json.toCharArray(jsonCharArray, 512);
	
	init(jsonCharArray);
}

AttackType::AttackType(char* json)
{
	init(json);
}

void AttackType::init(char* json)
{
	JsonParser<16> parser;
  JsonObject root = parser.parse(json);

  if (!root.success()) {
    HTTP_DEBUG_PRINTLN("JsonParser.parse() failed");
  }
	else {
		id = String((char*)root["id"]);
		name = String((char*)root["name"]);
		createdAt = String((char*)root["created_at"]);
		updatedAt = String((char*)root["updated_at"]);
		
		HTTP_DEBUG_PRINT("id: ");
		HTTP_DEBUG_PRINTLN(id);
		HTTP_DEBUG_PRINT("name: ");
		HTTP_DEBUG_PRINTLN(name);
		HTTP_DEBUG_PRINT("createdAt: ");
		HTTP_DEBUG_PRINTLN(createdAt);
		HTTP_DEBUG_PRINT("updatedAt: ");
		HTTP_DEBUG_PRINTLN(updatedAt);
	}
}

String AttackType::getId()
{
	return id;
}

String AttackType::getName()
{
	return name;
}

String AttackType::getCreatedAt()
{
	return createdAt;
}

String AttackType::getUpdatedAt()
{
	return updatedAt;
}
