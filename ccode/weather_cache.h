#ifndef _WEATHER_CACHE_H
#define  _WEATHER_CACHE_H

#include <stdlib.h>
#include <string.h>

typedef struct {
  char* query;
  char* msg;
  WeatherCacheNode *next;
}WeatherCacheNode;

WeatherCacheNode *makeNode(char* query, char* msg);
WeatherCacheNode *addNode(WeatherCacheNode *node, WeatherCacheNode *head);
int *searchCache(char* query);
int *freeNode(WeatherCacheNode *node);

#endif
