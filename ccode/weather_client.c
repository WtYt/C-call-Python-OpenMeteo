#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

int echo_client (char *server, int portno, const char* place, const char* date);
int fdopen_sock (int sock, FILE **inp, FILE **outp);

int main (int argc, char *argv[])
{
  char *server;
  int portno;
  if (argc != 5) {
    fprintf(stdout, "Usage: %s host port place y-m-d.\n", argv[0]);
    exit(-1);
  }
  server = argv[1];
  //printf("server %s\n", server);
  portno = strtol(argv[2], 0, 10);
  //printf("portno %d\n", portno);
  echo_client(server, portno, argv[3], argv[4]);
}

#define BUFFERSIZE 1024

int echo_client(char *server, int portno, const char* place, const char* date)
{
  char sbuf[BUFFERSIZE];
  char rbuf[BUFFERSIZE];
  int sock;
  FILE *in, *out;

  sock = tcp_connect (server, portno);
  if (sock < 0) {
    exit (-1);
  }

  if (fdopen_sock(sock, &in, &out) < 0) {
    fprintf(stderr, "fdopen()\n");
    exit(1);
  }
  //printf("==> ");
  fflush(stdout);

  /* gets buffer */
  char msg[100];
  sprintf(msg, "%s %s\n", place, date);
  //fgets(sbuf, BUFFERSIZE, stdin);
  //printf("%d chars\n", strlen(msg));
  /*
  if (strncasecmp(sbuf, "exit", 4) == 0) {
    printf("break\n");
    fprintf(stdout, "sending: [%s]\n", sbuf );
    fprintf(out, "%s", sbuf);
    break;
  }
  */

  //fprintf(stdout, "sending: [%s]\r\n", msg );
  fprintf(out, "%s", msg);
  //TODO read until end
  //printf("get\n");
  //printf("got\n");
  fgets(rbuf, BUFFERSIZE, in);
  for (size_t l = 0; l < strlen(rbuf); l++) {
    if (rbuf[l] == ' ') {
      rbuf[l] = '\n';
    }
  }
  printf("%s", rbuf);
  /*
  fgets(rbuf, BUFFERSIZE, in);
  printf("%s", rbuf);
  fgets(rbuf, BUFFERSIZE, in);
  printf("%s", rbuf);
  fgets(rbuf, BUFFERSIZE, in);
  printf("%s", rbuf);
  */
  //printf("==> ");
  fflush(stdout);
  //printf("connection closed\n");
  fclose(in);
  fclose(out);
}

int tcp_connect (char *server, int portno)
{
  struct addrinfo hints, *ai;
  struct sockaddr_in addr; /* sockaddr_storage */

  int s;
  int err;

  if (( s = socket (PF_INET, SOCK_STREAM, 0)) < 0) {
    perror("socket");
    return (-1);
  }
  //printf("socket() called\n");

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;

  if ((err = getaddrinfo(server, NULL, &hints, &ai))) {
    fprintf(stderr, "sockaddr too large (%d) > (%ld)\n",
            ai->ai_addrlen, sizeof(addr));

    freeaddrinfo(ai);
    close(s);
    return(-1);
  }
  //printf("getaddrinfo() called\n");

  memcpy (&addr, ai->ai_addr, ai->ai_addrlen);
  addr.sin_port = htons(portno);
  freeaddrinfo(ai);

  //printf("freeaddrinfo() called\n");

  if (connect(s, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror(server);
    close(s);
    return (-1);
  }

  //printf("connect() called\n");
  return(s);
}

int fdopen_sock(int sock, FILE **inp, FILE **outp)
{
  int sock2;
  if ((sock2 = dup(sock)) < 0)
  return (-1);
  //printf("dup() called\n");

  if ((*inp = fdopen(sock2, "r")) == NULL) {
    close (sock2);
    return (-1);
  }
  //printf("fdopen() called\n");

  if ((*outp = fdopen(sock, "w")) == NULL) {
    fclose (*inp);
    *inp = 0;
    return (-1);
  }

  setvbuf(*outp, (char *) NULL, _IOLBF, 0);
  return(0);
}
