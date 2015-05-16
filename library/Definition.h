
#ifndef __DEFINITION_H__
#define __DEFINITION_H__

#define DEBUG
//#define HTTP_DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(string) (Serial.print(string))
#endif

#ifdef DEBUG
#define DEBUG_PRINTLN(string) (Serial.println(string))
#endif

#ifndef DEBUG
#define DEBUG_PRINT(string)
#endif

#ifndef DEBUG
#define DEBUG_PRINTLN(string)
#endif


#ifdef HTTP_DEBUG
#define HTTP_DEBUG_PRINT(string) (Serial.print(string))
#endif

#ifdef HTTP_DEBUG
#define HTTP_DEBUG_PRINTLN(string) (Serial.println(string))
#endif

#ifndef HTTP_DEBUG
#define HTTP_DEBUG_PRINT(string)
#endif

#ifndef HTTP_DEBUG
#define HTTP_DEBUG_PRINTLN(string)
#endif

#endif
