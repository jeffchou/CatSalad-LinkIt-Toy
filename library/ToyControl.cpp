
#include "Definition.h"
#include "ToyControl.h"

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

ToyControl::ToyControl(String json)
{
	char jsonCharArray[512];
  json.toCharArray(jsonCharArray, 512);
	
	init(jsonCharArray);
}

ToyControl::ToyControl(char* json)
{
	init(json);
}

void ToyControl::init(char* json)
{
	JsonParser<32> parser;
  JsonArray root = parser.parse(json);

  if (!root.success()) {
    HTTP_DEBUG_PRINTLN("JsonParser.parse() failed");
    return;
  }
	else {
		if (root.size() == 0) {
			return;
		}
		
		id = String((char*)root[0]["id"]);
		battleId = String((char*)root[0]["battle_id"]);
		toyDeviceId = String((char*)root[0]["toy_device_id"]);
		type = String((char*)root[0]["type"]);
		
		HTTP_DEBUG_PRINT("id: ");
		HTTP_DEBUG_PRINTLN(id);
		HTTP_DEBUG_PRINT("battleId: ");
		HTTP_DEBUG_PRINTLN(battleId);
		HTTP_DEBUG_PRINT("toyDeviceId: ");
		HTTP_DEBUG_PRINTLN(toyDeviceId);
		HTTP_DEBUG_PRINT("type: ");
		HTTP_DEBUG_PRINTLN(type);
	}
}

String ToyControl::getId()
{
	return id;
}

String ToyControl::getBattleId()
{
	return battleId;
}

String ToyControl::getToyDeviceId()
{
	return toyDeviceId;
}

String ToyControl::getType()
{
	return type;
}

