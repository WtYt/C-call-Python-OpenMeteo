/*****************************************
* Auther : Yuta Watanabe
* Name   : weather_cache.c
* Date   : 2024-01-07
* About  : cache weather information
*****************************************/
#include "weather_cache.h"
#include <stdio.h>

WeatherCacheNode *makeNode(char* query, char* msg){
  printf("called makenode\n");
  WeatherCacheNode* node = (WeatherCacheNode*)malloc(sizeof(WeatherCacheNode));
  node->query = (char*)malloc(sizeof(char)*512);
  node->msg = (char*)malloc(sizeof(char)*10000);
  node->next = NULL;
  strcpy(node->query, query);
  strcpy(node->msg, msg);
  return node;
}

WeatherCacheNode *addNode(WeatherCacheNode* node, WeatherCacheNode** head){
  printf("called addnode\n");
  if(*head == NULL){
    *head = node;
    return *head;
  }
  WeatherCacheNode* n = *head;
  while(n->next != NULL){
    n = n->next;
  }
  n->next = node;
  return n;
}

WeatherCacheNode *searchCache(char* query, WeatherCacheNode* head){
  printf("called searchcache\n");
  if(head == NULL){
    printf("head is NULL in searchCache\n");
    return NULL;
  }
  WeatherCacheNode* node = head;
  while(node->next != NULL){
    printf("%s\n", query);
    printf("%s\n", node->query);
    if(strcmp(query, node->query) == 0){
      return node;
    }else{
      node = node->next;
    }
  }
  printf("%s\n", query);
  printf("%s\n", node->query);
  if(strcmp(query, node->query) == 0){
    return node;
  }else{
    node = node->next;
  }
  printf("not found NULL return searchCache\n");
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
