#include "weather_cache.h"

WeatherCacheNode *makeNode(char* query, char* msg){
  WeatherCacheNode* node = (WeatherCacheNode*)malloc(sizeof(WeatherCacheNode));
  node->query = (char*)malloc(sizeof(char)*512);
  node->msg = (char*)malloc(sizeof(char)*10000);
  node->next = NULL;
  strcpy(node->query, query);
  strcpy(node->msg, msg);
  return node;
}

WeatherCacheNode *addNode(WeatherCacheNode *node, WeatherCacheNode *head){
  head->next = node;
  return head;
}

WeatherCacheNode *searchCache(char* query, WeatherCacheNode* head){
  WeatherCacheNode* node = head;
  while(node->next != NULL){
    if(strcmp(query, head->query)){
      return node;
    }else{
      node = node->next;
    }
  }
  return NULL;
}

void freeCache(WeatherCacheNode *head){
  free(head->query);
  free(head->msg);
  if(head->next == NULL){
    free(head);
    return;
  }
  freeCache(head->next);
  free(head);
  return;
}
