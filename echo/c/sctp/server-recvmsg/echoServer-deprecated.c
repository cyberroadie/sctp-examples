#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/sctp.h>
#include <unistd.h>
#include "echoServer.h"

#define RECVBUFSIZE 2048

struct settings settings;

static void usage(void) {
  printf("-i <ip address/hostname>      server to connect to (default: 0.0.0.0)\n"
         "-p <port number>              port number to connect on (default: 4242)\n"
         "-m <message>                  message to send (default: recv)\n"
      );
  return;
}

int echoServer(char *host, int port, char *message) {

  int sd;

  sd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
  
  if(sd == -1) {
    perror("failure opeing socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in serverAddress;
  bzero((void *)&serverAddress, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET;                   
  serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);    
  serverAddress.sin_port = htons(port);

  if(-1 == bind(sd, (struct sockaddr *)&serverAddress, sizeof(serverAddress))) {
    perror("bind error");
    close(sd);
    exit(EXIT_FAILURE);
  }

  struct sctp_initmsg initMsg;
  bzero((void *)&initMsg, sizeof(initMsg));
  initMsg.sinit_num_ostreams = 100;
  initMsg.sinit_max_instreams = 100;
  initMsg.sinit_max_attempts = 0;
  initMsg.sinit_max_init_timeo = 0;

  setsockopt(sd, IPPROTO_SCTP, SCTP_INITMSG, &initMsg, sizeof(initMsg));

  if(listen(sd, 1) < 0) {
    perror("failed to lisen for connection");
    exit(EXIT_FAILURE);
  }

  printf("listening on port %d\n", port);

  int flags, n;
  struct sockaddr_in clientAddress;
  struct sctp_sndrcvinfo info;
  socklen_t len;
  char buffer[RECVBUFSIZE + 1] = {0};

  for(;;) {
    flags = 0;
    bzero((void*)&clientAddress, sizeof(clientAddress));
    bzero((void*)&info, sizeof(info));
    len = (socklen_t)sizeof(struct sockaddr_in);

    n = sctp_recvmsg(sd, (void*)buffer, RECVBUFSIZE,
                     (struct sockaddr *)&clientAddress, &len, &info, &flags);
    if(n == -1) {
      perror("error receiving message: ");
      continue;
    }
    
    printf("message received: %s\n", buffer);

  }

  return EXIT_SUCCESS;
}

static void settingsInit(void) {
  settings.host = "0.0.0.0";
  settings.port = 4242;
  settings.message = "recv";
  settings.verbose= false;
}

int main(int argc, char **argv) {
 
  int c;

  settingsInit();

  while(-1 != (c = getopt(argc, argv,
          "h:"
          "i:"
          "p:"
          "m:"
          "c:"
          "b:"
          "v"
          ))) {
    switch (c) {
      case 'h':
        usage();
        exit(EXIT_SUCCESS);
      case 'i':
        settings.host = optarg;
        break;
      case 'p':
        settings.port = atoi(optarg);
        break;
      case 'm':
        settings.message = optarg;
        break;
      case 'v':
        settings.verbose = true;
        break;
      default:
        fprintf(stderr, "Illegal argument \"%c\"\n", c);
        exit(EXIT_FAILURE);
    }
  }
  echoServer(settings.host, settings.port, settings.message);
}
