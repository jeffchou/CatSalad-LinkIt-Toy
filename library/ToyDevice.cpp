
#include "Arduino.h"

#include "Definition.h"
#include "ToyDevice.h"

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

ToyDevice::ToyDevice(String json)
{
	char jsonCharArray[512];
  json.toCharArray(jsonCharArray, 512);
	
	init(jsonCharArray);
}

ToyDevice::ToyDevice(char* json)
{
	init(json);
}

void ToyDevice::init(char* json)
{
	JsonParser<32> parser;
  JsonObject root = parser.parse(json);

  if (!root.success()) {
    HTTP_DEBUG_PRINTLN("JsonParser.parse() failed");
    return;
  }
	else {
		id = String((char*)root["id"]);
		enableSound = String((char*)root["is_enable_sound"]);
		useSystemRecommendedTime = String((char*)root["is_use_system_recommended_time"]);
		systemRecommendedTime = String((char*)root["system_recommended_time"]);
		userScheduledTime = String((char*)root["user_scheduled_time"]);
		location[0] = root["location"]["latitude"];
		location[1] = root["location"]["longitude"];
		
		HTTP_DEBUG_PRINT("id: ");
		HTTP_DEBUG_PRINTLN(id);
		HTTP_DEBUG_PRINT("isEnableSound: ");
		HTTP_DEBUG_PRINTLN(enableSound ? "true" : "false");
		HTTP_DEBUG_PRINT("isUseSystemRecommendedTime: ");
		HTTP_DEBUG_PRINTLN(useSystemRecommendedTime ? "true" : "false");
		HTTP_DEBUG_PRINT("SystemRecommededTime: ");
		HTTP_DEBUG_PRINTLN(systemRecommendedTime);
		HTTP_DEBUG_PRINT("userScheduledTime: ");
		HTTP_DEBUG_PRINTLN(userScheduledTime);
		HTTP_DEBUG_PRINT("latitude: ");
		HTTP_DEBUG_PRINTLN(location[0]);
		HTTP_DEBUG_PRINT("longitude: ");
		HTTP_DEBUG_PRINTLN(location[1]);
	}
}

String ToyDevice::getId()
{
	return id;
}

bool ToyDevice::isEnableSound()
{
	return enableSound;
}

bool ToyDevice::isUseSystemRecommendedTime()
{
	return useSystemRecommendedTime;
}

String ToyDevice::getSystemRecommendedTime()
{
	return systemRecommendedTime;
}

String ToyDevice::getUserScheduledTime()
{
	return userScheduledTime;
}

double* ToyDevice::getLocation()
{
	return location;
}
