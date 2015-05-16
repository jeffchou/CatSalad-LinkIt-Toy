#ifndef __USER_H__
#define __USER_H__

#include <Arduino.h>

class User {
private:
	String id;
	String name;
	String catId;
	String createdAt;
	String updatedAt;
	
	void init(char* json);
	
public:
	User(String json);
	User(char* json);
	
	String getId();
	String getName();
	String getCatId();
	String getCreatedAt();
	String getUpdatedAt();
};

#endif
