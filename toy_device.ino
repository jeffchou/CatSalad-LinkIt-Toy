#include <Time.h>
#include <TimeAlarms.h>
#include <LDateTime.h>

#include <LTask.h>
#include <LWiFi.h>
#include <LWiFiClient.h>
#include <WifiRestClient.h>

#include <JsonParser.h>
#include <Thread.h>
#include <ThreadController.h>

#include <Cat.h>
#include <Definition.h>
#include <EquippedWeapon.h>
#include <RESTfulService.h>
#include <ToyDevice.h>
#include <User.h>
#include <Battle.h>
#include <ToyControl.h>
#include <Boss.h>
#include <AttackType.h>

using namespace ArduinoJson::Parser;

#include <Servo.h> 
#define ard_log Serial.printf

int i = 0 ;
Servo myservo;

// G-Sensor X-axis
#define I0 A0
// G-Sensor Y-axis
#define I1 A1
#define NORMAL_LED  13

// These constants won't change. They're used to give names
// to the pins used:
const int analogInPin1 = I0; // Analog input pin that the Accelerometer's first pin is attached to
const int analogInPin2 = I1; // Analog input pin that the Accelerometer's second pin is attached to
const int threshold_X = 485;
const int threshold_Y = 485;

int setPosition(int pos)
{
    if (pos<0)
    {
      pos = 0;;
    }
    else if (pos>360)
    {
      pos = 360;
    }    
    myservo.write(pos);
}

void getSensorData(int *sensorValueX, int *sensorValueY)
{
    int rawValueX = 0, rawValueY = 0;
    // read the both analog in values:
    rawValueX = analogRead(analogInPin1); 
    rawValueY = analogRead(analogInPin2);   
		
    *sensorValueX = rawValueX - threshold_X;
		*sensorValueY = rawValueY - threshold_Y;
		
    if (*sensorValueX == 0 && *sensorValueY == 0)
    {
       digitalWrite(NORMAL_LED, HIGH);      
    }
    else
    {
        digitalWrite(NORMAL_LED, LOW);     
    }
    /*
    // read the both analog in values:
    *sensorValueX = analogRead(analogInPin1); 
    *sensorValueY = analogRead(analogInPin2); 
    */
    // wait 10 milliseconds before the next loop
    // for the analog-to-digital converter to settle
    // after the last reading:
    // delay(10);
}


#define WIFI_AP "SSID"
#define WIFI_PASSWORD "PASSWORD"
#define WIFI_AUTH LWIFI_WPA  // choose from LWIFI_OPEN, LWIFI_WPA, or LWIFI_WEP.

const char* HostName = "HOSTNAME";
const int Port = 80;

char* toy_device_id = "device_id";

String response;
WifiRestClient* client;

RESTfulService* service = NULL;
ToyDevice* toyDevice = NULL;
User* user = NULL;
Cat* cat = NULL;
Battle* activeBattle = NULL;
Boss* boss = NULL;
AttackType* attackTypes = NULL;

boolean useSystemRecommendedTime = true;
datetimeInfo systemRecommendedTime;
datetimeInfo userScheduledTime;

AlarmID_t toyScheduleAlarmId = -1;
int amountOfAttackTypes = 0;

Thread updateToyDeviceScheduleTask;
Thread sensorUpdateTask;
Thread systemTimeUpdateTask;
Thread ledControlTask;
Thread catPlayTask;
Thread remoteToyControlTask;

int _100MS = 100;
int _200MS = 200;
int _500MS = 500;
int _1SECS = 1000;
int _5SECS = 5000;
int _10SECS = 10000;
int _30SECS = 30000;
int _60SECS = 60000;

int _5MINS = 300000;
int _10MINS = 600000;
int _30MINS = 1800000;

int sensorX = 0;
int sensorY = 0;

// To be defined
double meaningfulActiveValueThreshole = 25;
double playingActiveValueThreashold = 30;

double prevActiveValue = 0;
boolean isPlaying = false;
int continueousStopPlayingNumber = 0;
const int MaxStopPlayingNumber = 30;
boolean isControlled = false;

void getToyDevice() {
	if (toyDevice != NULL) {
		delete toyDevice;
		toyDevice = NULL;
	}
	toyDevice = service->getToyDevice(toy_device_id);
	useSystemRecommendedTime = toyDevice->isUseSystemRecommendedTime();
	systemRecommendedTime = parseDateTime(toyDevice->getSystemRecommendedTime());
	userScheduledTime = parseDateTime(toyDevice->getUserScheduledTime());
	
	scheduleToyPlay();
}

void getUser() {
	if (user != NULL) {
		delete user;
		user = NULL;
	}
	user = service->getUserByToyDeviceId(toy_device_id);
}

void getCat() {
	cat = service->getCatByCatId(user->getCatId().c_str());
}

void getActiveBattle() {
	if (activeBattle != NULL) {
		delete activeBattle;
		activeBattle = NULL;
	}
	activeBattle = service->getActiveBattle();
	getBossByBattleId(activeBattle->getId().c_str());
}

void joinBattle() {
	service->joinBattle(activeBattle->getId().c_str(), cat->getId().c_str());
}

void createActivity(String hitType) {
	// TODO: Implement the current datetime
	String datetime = "";
	
	boolean success = service->createActivity(activeBattle->getId().c_str(), cat->getId().c_str(), datetime, hitType);
	if (success) {
		DEBUG_PRINTLN("Success to create activity.");
	}
}

void toyControl() {
	ToyControl* toyControl = service->getToyControl(activeBattle->getId().c_str(), toy_device_id);
	
	if (toyControl == NULL) {
		if (isControlled) {
			stopMotor();
		}
		isControlled = false;
	}
	else {
		isPlaying = false;
	
		String type = toyControl->getType();
		if (type == "clockwise") {
			clockwiseMotor();
		}
		else if (type == "counterclockwise") {
			counterclockwiseMotor();
		}
		else {
			stopMotor();
		}
		
		isControlled = true;
	}
	
	delete toyControl;
}

void getBossByBattleId(const char* battleId) {
	if (boss != NULL) {
		delete boss;
		boss = NULL;
	}
	boss = service->getBossByBattleId(activeBattle->getId().c_str());
	getAttackTypesByPatternId(boss->getAttackPatternId().c_str());
}

void getAttackTypesByPatternId(const char* patternId) {
	if (attackTypes != NULL) {
		delete [] attackTypes;
	}
	attackTypes = service->getAttackTypesByPatternId(patternId, &amountOfAttackTypes);
	
	for (int i = 0; i < amountOfAttackTypes; ++i) {
		AttackType type = attackTypes[i];
		DEBUG_PRINT("attack type name: ");
		DEBUG_PRINTLN(type.getName());
	}
}

void updateToyDeviceScheduleTime() {
	getToyDevice();
}

boolean checkPlaying() {
	if (isControlled) {
		return false;
	}
	
	boolean playNow = isPlaying;
	double activeValue = calculateActiveValue();
	if (activeValue >= playingActiveValueThreashold) {
		playNow = true;
		continueousStopPlayingNumber = 0;
		createActivity("hit");
	}
	else if (activeValue >= meaningfulActiveValueThreshole && activeValue < playingActiveValueThreashold) {
		if (playNow) {
			createActivity("combo");
		}
	}
	else {
		++continueousStopPlayingNumber;
		DEBUG_PRINT("Stop playing number: ");
		DEBUG_PRINTLN(continueousStopPlayingNumber);
		
		if (continueousStopPlayingNumber > MaxStopPlayingNumber) {
			playNow = false;
		}
	}
	return playNow;
}

void updateGSensorData() {
	int x = 0;
	int y = 0;
	getSensorData(&x, &y);
	DEBUG_PRINT("accelerometer X = " ); 
	DEBUG_PRINT(x); 
	DEBUG_PRINT("\t accelerometer Y = " ); 
	DEBUG_PRINT(y);
	DEBUG_PRINT("\t active value = ");
	DEBUG_PRINTLN(calculateActiveValue());
	
	sensorX = x;
	sensorY = y;
	
	boolean playing = checkPlaying();
	if (playing && !isPlaying) {
		DEBUG_PRINTLN("Auto Motor Play.");
		isPlaying = true;
		return;
	}
	if (!playing) {
		DEBUG_PRINTLN("Disable Auto Motor Play.");
		isPlaying = false;
	}
}

datetimeInfo parseDateTime(String datetime) {
	datetimeInfo t;
	char buffer[8];
	t.year = atoi(datetime.substring(0, 4).c_str());
	t.mon = atoi(datetime.substring(5, 7).c_str());
	t.day = atoi(datetime.substring(8, 10).c_str());
	t.hour = atoi(datetime.substring(11, 13).c_str());
	t.min = atoi(datetime.substring(14, 16).c_str());
	t.sec = atoi(datetime.substring(17, 19).c_str());
	
	return t;
}

void updateSystemTime() {
	String datetime = service->getSystemTime();
	datetimeInfo t = parseDateTime(datetime);
	LDateTime.setTime(&t);
	
	DEBUG_PRINT("Now Time: ");
	DEBUG_PRINTLN(datetime);
	
	if (t.hour < 1 && t.min < 5) {
		getActiveBattle();
	}
}

double calculateActiveValue() {
	return sqrt(sensorX*sensorX + sensorY*sensorY);
}

int executingAttackType = 0;

void catPlayTime() {
	if (!isPlaying) {
		if (!isControlled) {
			stopMotor();
		}
		return;
	}
	
	// Auto Moto Play
	AttackType type = attackTypes[executingAttackType];
	if (type.getName() == "clockwise") {
		clockwiseMotor();
	}
	else if (type.getName() == "counterclockwise") {
		counterclockwiseMotor();
	}
	else {
		stopMotor();
	}
	
	executingAttackType = (executingAttackType + 1) % amountOfAttackTypes;
}

void scheduledMotorPlay() {
	DEBUG_PRINTLN("Scheduled Motor Play.");
	
	int hour = systemRecommendedTime.hour;
	int minute = systemRecommendedTime.min;
	int second = systemRecommendedTime.sec;
	char buffer[128];
	sprintf(buffer, "Now is scheduled play time: %d:%d:%d", hour, minute, second);
	DEBUG_PRINTLN(buffer);
	
	isPlaying = true;
}

void scheduleToyPlay() {
	datetimeInfo t;
	if (useSystemRecommendedTime) {
		t = systemRecommendedTime;
	}
	else {
		t = userScheduledTime;
	}
	
	int hour = t.hour;
	int minute = t.min;
	int second = t.sec;
	if (toyScheduleAlarmId != -1) {
		Alarm.disable(toyScheduleAlarmId);
	}
	toyScheduleAlarmId = Alarm.alarmRepeat(hour, minute, second, scheduledMotorPlay);
}

void clockwiseMotor() {
	DEBUG_PRINTLN("clockwise motor");
	myservo.attach(9);
	setPosition(0);
}

void counterclockwiseMotor() {
	DEBUG_PRINTLN("counterclockwise motor");
	myservo.attach(9);
	setPosition(180);
}

void stopMotor() {
	DEBUG_PRINTLN("stop motor");
	pinMode(9, INPUT);
}

int led = 0;

void ledControl() {
	if (led == 0) {
		digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
	else {
		digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
	}
	led = (led + 1) % 2;
}

void setup() {
  Serial.begin(115200);
  pinMode(13, OUTPUT);
	
	client = new WifiRestClient(HostName, Port);
	service = new RESTfulService(client);
	
	updateSystemTime();
	getToyDevice();
	getUser();
	getCat();
	getActiveBattle();
	joinBattle();
	
	ledControlTask = Thread(ledControl, _1SECS);
	updateToyDeviceScheduleTask = Thread(updateToyDeviceScheduleTime, _60SECS);
	sensorUpdateTask = Thread(updateGSensorData, _200MS);
	
	remoteToyControlTask = Thread(toyControl, _1SECS);
	catPlayTask = Thread(catPlayTime, _5SECS);
}

void loop() {
	if (ledControlTask.shouldRun()) {
		ledControlTask.run();
	}
	if (sensorUpdateTask.shouldRun()) {
		sensorUpdateTask.run();
	}
	if (updateToyDeviceScheduleTask.shouldRun()) {
		updateToyDeviceScheduleTask.run();
	}
	if (remoteToyControlTask.shouldRun()) {
		remoteToyControlTask.run();
	}
	if (catPlayTask.shouldRun()) {
		catPlayTask.run();
	}
}



