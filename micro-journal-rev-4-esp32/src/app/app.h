#ifndef App_h
#define App_h

#include <ArduinoJson.h>

// app version
#define VERSION "1.0.16.r4.be"

// app status
JsonDocument &app_status();

//
void app_log(const char *format, ...);
String format(const char *format, ...);

#endif