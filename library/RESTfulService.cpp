
#include "Arduino.h"

#include <LWiFi.h>
#include <LWiFiClient.h>
#include <WifiRestClient.h>

#include <JsonParser.h>

using namespace ArduinoJson::Parser;

#include "Definition.h"
#include "RESTfulService.h"

#include "ToyDevice.h"
#include "User.h"
#include "Cat.h"
#include "Battle.h"
#include "ToyControl.h"
#include "Boss.h"
#include "AttackType.h"

#define WIFI_AP "WIFI Access Point"
#define WIFI_PASSWORD "Password"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.

String SystemTimeRoute = "/v1/server_time";
String ToyDeviceRoute = "/v1/toy_device";
String UserRoute = "/v1/user";
String CatRoute = "/v1/cat";
String BattleRoute = "/v1/battle";
String BossRoute = "/boss";
String ParticipantRoute = "/participant";
String ActivityRoute = "/activity";
String ToyControlChildRoute = "/control";
String ToyDeviceChildRoute = "/toy_device";
String AttackPatternRoute = "/v1/attack_pattern";
String AttackTypeRoute = "attack_type";
String ContentRoute = "/content";

RESTfulService::RESTfulService(WifiRestClient* client)
{
	this->client = client;
}

RESTfulService::RESTfulService(const char* hostName, int port)
{
	this->hostName = hostName;
	this->port = port;
	
	this->client = new WifiRestClient(hostName, port);
}

char* RESTfulService::getSystemTime()
{
	String apiResponse;
	String requestUrl = SystemTimeRoute;
	int statusCode = sendHttpGetRequest(requestUrl, &apiResponse);
	if (statusCode != 200) {
		return NULL;
	}
	if (strlen(apiResponse.c_str()) == 0) {
		return NULL;
	}
	char* datetime = NULL;
	
	char jsonCharArray[64];
  apiResponse.toCharArray(jsonCharArray, 64);
	JsonParser<8> parser;
  JsonObject root = parser.parse(jsonCharArray);
  if (!root.success()) {
    HTTP_DEBUG_PRINTLN("JsonParser.parse() failed");
  }
	else {
		datetime = root["datetime"];
		HTTP_DEBUG_PRINT("datetime: ");
		HTTP_DEBUG_PRINTLN(datetime);
	}
	
	return datetime;
}

ToyDevice* RESTfulService::getToyDevice(const char* toyDeviceId)
{
	String apiResponse;
	String requestUrl = ToyDeviceRoute + "/" + toyDeviceId;
	int statusCode = sendHttpGetRequest(requestUrl, &apiResponse);
	if (statusCode != 200) {
		return NULL;
	}
	return new ToyDevice(apiResponse);
}

User* RESTfulService::getUserByToyDeviceId(const char* toyDeviceId)
{
	String apiResponse;
	String requestUrl = ToyDeviceRoute + "/" + toyDeviceId + "/user";
	int statusCode = sendHttpGetRequest(requestUrl, &apiResponse);
	if (statusCode != 200) {
		return NULL;
	}
	return new User(apiResponse);
}

Cat* RESTfulService::getCatByCatId(const char* catId)
{
	String apiResponse;
	String requestUrl	= CatRoute + "/" + catId + "?type=simple";
	int statusCode = sendHttpGetRequest(requestUrl, &apiResponse);
	if (statusCode != 200) {
		return NULL;
	}
	return new Cat(apiResponse);
}

Battle* RESTfulService::getActiveBattle()
{
	String apiResponse;
	String requestUrl = BattleRoute + "?filter=active&content_type=simple";
	int statusCode = sendHttpGetRequest(requestUrl, &apiResponse);
	if (statusCode != 200) {
		return NULL;
	}
	return new Battle(apiResponse);
}

Cat* RESTfulService::joinBattle(const char* battleId, const char* catId)
{
	String apiResponse;
	String requestUrl = BattleRoute + "/" + battleId + ParticipantRoute;
	String bodyString = String("{\"cat_id\":\"");
	bodyString.concat(catId);
	bodyString.concat("\"}");
	
	int statusCode = sendHttpPostRequest(requestUrl, bodyString, &apiResponse);
	if (statusCode != 201) {
		return NULL;
	}
	return NULL;
}

boolean RESTfulService::createActivity(const char* battleId, const char* catId, String nowTime, String type)
{
	String apiResponse;
	String requestUrl = BattleRoute + "/" + battleId + ParticipantRoute + "/" + catId + ActivityRoute;
	String bodyString = "{\"time\":\"" + nowTime + "\",\"hit_type\":\"" + type + "\"}";
	
	Serial.print("bodyString: ");
	Serial.println(bodyString);
	int statusCode = sendHttpPostRequest(requestUrl, bodyString, &apiResponse);
	if (statusCode != 201) {
		return true;
	}
	return false;
}

ToyControl* RESTfulService::getToyControl(const char* battleId, const char* toyDeviceId)
{
	String apiResponse;
	String requestUrl = BattleRoute + "/" + battleId + ToyDeviceChildRoute + "/" + toyDeviceId + ToyControlChildRoute + "?content_type=simple";
	int statusCode = sendHttpGetRequest(requestUrl, &apiResponse);
	if (statusCode != 200) {
		return NULL;
	}
	if (strlen(apiResponse.c_str()) == 2) {
		return NULL;
	}
	return new ToyControl(apiResponse);
}

Boss* RESTfulService::getBossByBattleId(const char* battleId)
{
	String apiResponse;
	String requestUrl = BattleRoute + "/" + battleId + BossRoute + "?content_type=simple";
	int statusCode = sendHttpGetRequest(requestUrl, &apiResponse);
	if (statusCode != 200) {
		return NULL;
	}
	if (strlen(apiResponse.c_str()) == 0) {
		return NULL;
	}
	return new Boss(apiResponse);
}

AttackType* RESTfulService::getAttackTypesByPatternId(const char* patternId, int* size)
{
	String apiResponse;
	String requestUrl = AttackPatternRoute + "/" + patternId + ContentRoute + "?content_type=simple";
	int statusCode = sendHttpGetRequest(requestUrl, &apiResponse);
	if (statusCode != 200) {
		return NULL;
	}
	if (strlen(apiResponse.c_str()) == 0) {
		return NULL;
	}
	
	char jsonCharArray[512];
  apiResponse.toCharArray(jsonCharArray, 512);
	
	JsonParser<32> parser;
  JsonArray root = parser.parse(jsonCharArray);
  if (!root.success()) {
    HTTP_DEBUG_PRINTLN("JsonParser.parse() failed");
  }
	else {
		int array_size = root.size();
		*size = array_size;
		AttackType* types = new AttackType[array_size];
		for (int i = 0; i < array_size; ++i) {
			char* id = root[i]["id"];
			char* name = root[i]["name"];
			char* createdAt = root[i]["created_at"];
			char* updatedAt = root[i]["updated_at"];
			types[i] = AttackType(id, name, createdAt, updatedAt);
		}
		return types;
	}
	return NULL;
}

int RESTfulService::sendHttpGetRequest(String url, String* response)
{
	int status = this->client->begin(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD));
	this->client->setHeader("Accept: application/json");
#ifdef DEBUG
	Serial.print("client begin status: ");
  Serial.println(status);
#endif

	char urlString[512];
  url.toCharArray(urlString, 512);
#ifdef DEBUG
	Serial.print("Request API URL: ");
	Serial.println(urlString);
#endif

  *response = "";
  int statusCode = this->client->get(urlString, response);
#ifdef DEBUG
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("Response body from server: ");
  Serial.println(*response);
#endif

	return statusCode;
}

int RESTfulService::sendHttpPostRequest(String url, String* response)
{
	return sendHttpPostRequest(url, String(), response);
}

int RESTfulService::sendHttpPostRequest(String url, String bodyString, String* response)
{
	int status = this->client->begin(WIFI_AP, LWiFiLoginInfo(WIFI_AUTH, WIFI_PASSWORD));
	this->client->setHeader("Accept: application/json");
	this->client->setHeader("Content-Type: application/json");
#ifdef DEBUG
	Serial.print("client begin status: ");
  Serial.println(status);
#endif

	char urlString[512];
  url.toCharArray(urlString, 512);
#ifdef DEBUG
	Serial.print("Request API URL: ");
	Serial.println(urlString);
#endif

	char body[512];
	bodyString.toCharArray(body, 512);
	
  *response = "";
  int statusCode = this->client->post(urlString, body, response);
#ifdef DEBUG
  Serial.print("Status code from server: ");
  Serial.println(statusCode);
  Serial.print("Response body from server: ");
  Serial.println(*response);
#endif

	return statusCode;
}
