
#include "Definition.h"
#include "User.h"

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

User::User(String json)
{
	char jsonCharArray[512];
  json.toCharArray(jsonCharArray, 512);
	
	init(jsonCharArray);
}

User::User(char* json)
{
	init(json);
}

void User::init(char* json)
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
		catId = String((char*)root["cat_id"]);
		createdAt = String((char*)root["created_at"]);
		updatedAt = String((char*)root["updated_at"]);
		
		HTTP_DEBUG_PRINT("id: ");
		HTTP_DEBUG_PRINTLN(id);
		HTTP_DEBUG_PRINT("name: ");
		HTTP_DEBUG_PRINTLN(name);
		HTTP_DEBUG_PRINT("catId: ");
		HTTP_DEBUG_PRINTLN(catId);
		HTTP_DEBUG_PRINT("createdAt: ");
		HTTP_DEBUG_PRINTLN(createdAt);
		HTTP_DEBUG_PRINT("updatedAt: ");
		HTTP_DEBUG_PRINTLN(updatedAt);
	}
}

String User::getId()
{
	return id;
}

String User::getName()
{
	return name;
}

String User::getCatId()
{
	return catId;
}

String User::getCreatedAt()
{
	return createdAt;
}

String User::getUpdatedAt()
{
	return updatedAt;
}
