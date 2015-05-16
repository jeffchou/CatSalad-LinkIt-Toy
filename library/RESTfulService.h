#ifndef __RESTFUL_SERVICE_H__
#define __RESTFUL_SERVICE_H__

#include <Arduino.h>

class ToyDevice;
class User;
class Cat;
class Battle;
class ToyControl;
class Boss;
class AttackType;

class WifiRestClient;

class RESTfulService {
private:
	const char* hostName;
	int port;
	WifiRestClient* client;
	
	// GET
	int sendHttpGetRequest(String url, String* response);
	
	// POST
	int sendHttpPostRequest(String url, String* response);
	int sendHttpPostRequest(String url, String bodyString, String* response);
	
public:
	RESTfulService(WifiRestClient* client);
	RESTfulService(const char* hostName, int port);
	
	char* getSystemTime();
	ToyDevice* getToyDevice(const char* toyDeviceId);
	User* getUserByToyDeviceId(const char* toyDeviceId);
	Cat* getCatByCatId(const char* catId);
	Battle* getActiveBattle();
	Cat* joinBattle(const char* battleId, const char* catId);
	boolean createActivity(const char* battleId, const char* catId, String nowTime, String type);
	ToyControl* getToyControl(const char* battleId, const char* toyDeviceId);
	Boss* getBossByBattleId(const char* battleId);
	AttackType* getAttackTypesByPatternId(const char* patternId, int* size);
};

#endif
