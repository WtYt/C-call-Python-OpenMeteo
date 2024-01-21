/*****************************************
* Auther : Yuta Watanabe
* Name   : weather_client.h
* Date   : 2024-01-07
* About  : weather client
*****************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <Python.h>
#include "pyMeteo.h"
#include "weather_cache.h"

#define MAX_CLIENTS 10

void *handle_client(void *arg);

WeatherCacheNode* head = NULL;

pthread_mutex_t mutex;

int main (int argc, char *argv[])
{
  int sockfd, new_sockfd;
  struct sockaddr_in serv, clnt;
  socklen_t sin_siz;

  int port, i;
  if(argc != 3){
    printf("Usage: ./prog host port\n");
    exit(EXIT_FAILURE);
  }
  if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
    perror("socket");
    exit(EXIT_FAILURE);
  }
  Py_Initialize();
  //printf("socket() called\n");
  serv.sin_family = PF_INET;
  port = strtol(argv[2], NULL, 10);
  serv.sin_port = htons(port);
  inet_aton(argv[1], &serv.sin_addr);
  sin_siz = sizeof(struct sockaddr_in);
  if(bind(sockfd, (struct sockaddr*)&serv, sizeof(serv)) < 0){
    perror("bind");
    exit(EXIT_FAILURE);
  }
  //printf("bind() called\n");
  if(listen(sockfd, SOMAXCONN) < 0){
    perror("listen");
    exit(EXIT_FAILURE);
  }
  //printf("listen() called\n");
  while(1){
    if((new_sockfd = accept(sockfd, (struct sockaddr *)&clnt, &sin_siz)) < 0){
      perror("accept");
      exit(EXIT_FAILURE);
    }

    pthread_t t;
    if (pthread_create(&t, NULL, handle_client, (void *)(intptr_t)new_sockfd) != 0){
      perror("pthread_create");
      exit(EXIT_FAILURE);
    }
    pthread_detach(t);
  }
  close(sockfd);
  Py_Finalize();
  freeCache(head);
  return 0;
}

void *handle_client(void *arg){
  int new_sockfd = (intptr_t)arg;
  char buf[BUFSIZ];
  int len, year, month, day, flag;
  char name[BUFSIZ];
  char msg[10000];
  char method[16];
  char version[16];
  MeteoWeather* weather;
  memset(buf, 0, BUFSIZ);
  len = recv(new_sockfd, buf, BUFSIZ, 0);
  printf("len = %d\n", len);
  if (len == 2) {
    printf("buf[0] = %d\n", buf[0]);
    printf("buf[1] = %d\n", buf[1]);
    printf("buf[2] = %d\n", buf[2]);
  }
  buf[len] = '\0';
  if (buf[len-1] == '\n') {
    if (buf[len-2] == '\r') {
      buf[len-2] = '\0';
    }
    buf[len-1] = '\0';
  }
  if (len == 2) {
    printf("buf[0] = %d\n", buf[0]);
    printf("buf[1] = %d\n", buf[1]);
    printf("buf[2] = %d\n", buf[2]);
  }
  printf("recieved %s\n", buf);
  // request is "name y-m-d" style
  flag = sscanf(buf, "%s %d-%d-%d", name, &year, &month, &day);
  if(flag != 4){
    printf("invalid request \"%s\"\n", buf);
    sprintf(msg, "invalid request \"%s\"\n", buf);
    len = send (new_sockfd, msg, 18, 0);
    printf("close connection\n");
    close(new_sockfd);
    pthread_exit(NULL);
  }
  else{
    printf("got weather\n");
    /*
    when some threads invoke same python module,
    critical section is required.
    */
    WeatherCacheNode *n;
    pthread_mutex_lock(&mutex);
    if((n = searchCache(buf, head)) == NULL){
      printf("not found in cache\n");
      weather = pyMeteo(name, year, month, day);
      // create message
      sprintf(msg, "%04d年の%02d月%02d日の%sの天候をお知らせします 天気は%sです 最高気温は%.1lf℃です 最低気温は%.1lf℃です\n",
      year, month, day, weather->fullname, weather->weather_msg, weather->max_temperature, weather->min_temperature);
      free(weather->fullname);
      free(weather->weather_msg);
      n = makeNode(buf, msg);
      addNode(n, &head);
    }
    else{
      // copy message from cache
      strcpy(msg, n->msg);
    }
    pthread_mutex_unlock(&mutex);
    printf("return\n%s", msg);
    len = send (new_sockfd, msg, 10000, 0);
  }
  printf("close connection");
  close(new_sockfd);
  pthread_exit(NULL);
}
