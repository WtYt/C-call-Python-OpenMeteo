#ifndef _WEATHER_CACHE_H
#define  _WEATHER_CACHE_H

#include <stdlib.h>
#include <string.h>

typedef struct wCacheNode WeatherCacheNode;

struct wCacheNode{
  char* query;
  char* msg;
  WeatherCacheNode *next;
};

WeatherCacheNode *makeNode(char* query, char* msg);
WeatherCacheNode *addNode(WeatherCacheNode *node, WeatherCacheNode **head);
WeatherCacheNode *searchCache(char* query, WeatherCacheNode* head);
void freeCache(WeatherCacheNode *node);

#endif
