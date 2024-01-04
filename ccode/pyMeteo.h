#ifndef _PYMETEO_H
#define _PYMETEO_H

#define PY_STR_BUFFSIZE 128

typedef struct {
  char*  fullname;
  char*  weather_msg;
  int    weather_code;
  double max_temperature;
  double min_temperature;
}MeteoWeather;

MeteoWeather *pyMeteo(const char *name, int year, int month, int day);

#endif
