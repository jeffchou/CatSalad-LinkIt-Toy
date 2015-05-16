
#include "Definition.h"
#include "Battle.h"

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

Battle::Battle(String json)
{
	char jsonCharArray[512];
  json.toCharArray(jsonCharArray, 512);
	
	init(jsonCharArray);
}

Battle::Battle(char* json)
{
	init(json);
}

void Battle::init(char* json)
{
	JsonParser<16> parser;
  JsonArray root = parser.parse(json);

  if (!root.success()) {
    HTTP_DEBUG_PRINTLN("JsonParser.parse() failed");
    return;
  }
	else {
		id = String((char*)root[0]["id"]);
		name = String((char*)root[0]["name"]);
		activatedAt = String((char*)root[0]["activated_at"]);
		createdAt = String((char*)root[0]["created_at"]);
		updatedAt = String((char*)root[0]["updated_at"]);
		
		HTTP_DEBUG_PRINT("id: ");
		HTTP_DEBUG_PRINTLN(id);
		HTTP_DEBUG_PRINT("name: ");
		HTTP_DEBUG_PRINTLN(name);
		HTTP_DEBUG_PRINT("activatedAt: ");
		HTTP_DEBUG_PRINTLN(activatedAt);
		HTTP_DEBUG_PRINT("createdAt: ");
		HTTP_DEBUG_PRINTLN(createdAt);
		HTTP_DEBUG_PRINT("updatedAt: ");
		HTTP_DEBUG_PRINTLN(updatedAt);
	}
}

String Battle::getId()
{
	return id;
}

String Battle::getName()
{
	return name;
}

String Battle::getActivatedAt()
{
	return activatedAt;
}

String Battle::getCreatedAt()
{
	return createdAt;
}

String Battle::getUpdatedAt()
{
	return updatedAt;
}
