
#include "Definition.h"
#include "AttackPattern.h"

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

AttackPattern::AttackPattern(String json)
{
	char jsonCharArray[512];
  json.toCharArray(jsonCharArray, 512);
	
	init(jsonCharArray);
}

AttackPattern::AttackPattern(char* json)
{
	init(json);
}

void AttackPattern::init(char* json)
{
	JsonParser<16> parser;
  JsonArray root = parser.parse(json);

  if (!root.success()) {
    HTTP_DEBUG_PRINTLN("JsonParser.parse() failed");
  }
	else {
		id = String((char*)root[0]["id"]);
		name = String((char*)root[0]["name"]);
		createdAt = String((char*)root[0]["created_at"]);
		updatedAt = String((char*)root[0]["updated_at"]);
		
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

String AttackPattern::getId()
{
	return id;
}

String AttackPattern::getName()
{
	return name;
}

String AttackPattern::getCreatedAt()
{
	return createdAt;
}

String AttackPattern::getUpdatedAt()
{
	return updatedAt;
}
