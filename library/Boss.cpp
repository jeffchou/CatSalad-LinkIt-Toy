
#include "Definition.h"
#include "Boss.h"

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

Boss::Boss(String json)
{
	char jsonCharArray[512];
  json.toCharArray(jsonCharArray, 512);
	
	init(jsonCharArray);
}

Boss::Boss(char* json)
{
	init(json);
}

void Boss::init(char* json)
{
	JsonParser<32> parser;
  JsonArray root = parser.parse(json);

  if (!root.success()) {
    HTTP_DEBUG_PRINTLN("JsonParser.parse() failed");
  }
	else {
		id = String((char*)root[0]["id"]);
		name = String((char*)root[0]["name"]);
		attackPatternId = String((char*)root[0]["attack_pattern_id"]);
		createdAt = String((char*)root[0]["created_at"]);
		updatedAt = String((char*)root[0]["updated_at"]);
		
		HTTP_DEBUG_PRINT("id: ");
		HTTP_DEBUG_PRINTLN(id);
		HTTP_DEBUG_PRINT("name: ");
		HTTP_DEBUG_PRINTLN(name);
		HTTP_DEBUG_PRINT("attackPatternId: ");
		HTTP_DEBUG_PRINTLN(attackPatternId);
		HTTP_DEBUG_PRINT("createdAt: ");
		HTTP_DEBUG_PRINTLN(createdAt);
		HTTP_DEBUG_PRINT("updatedAt: ");
		HTTP_DEBUG_PRINTLN(updatedAt);
	}
}

String Boss::getId()
{
	return id;
}

String Boss::getName()
{
	return name;
}

String Boss::getAttackPatternId()
{
	return attackPatternId;
}

String Boss::getCreatedAt()
{
	return createdAt;
}

String Boss::getUpdatedAt()
{
	return updatedAt;
}
