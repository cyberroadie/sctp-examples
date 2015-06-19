#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* optarg */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h> /* TCP_NODELAY lives here */
#include <netinet/sctp.h> /* SCTP_NODELAY lives here */
#include <pthread.h>
#include <stdbool.h>
#include "echoClient.h"

#define PPID 424242

typedef struct messageSender_data {
  char *host;
  int port;
  char *message;
  long count;
  int thread_id;
} messageSenderData_t;

struct settings settings;

void *messageSender(void *arg) {

  messageSenderData_t *data = (messageSenderData_t *) arg;

  char *message = strdup(data->message);
  int count = data->count;
  int sd;

  sd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
  
  if(sd == -1) {
    perror("failure opeing socket");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in  address;
  bzero((void*)&address, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(data->port);
  address.sin_addr.s_addr = inet_addr(data->host);

  struct sctp_initmsg initmsg = {0};
  initmsg.sinit_num_ostreams = 1;
  setsockopt(sd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg));

  for(int i = 0; i < count; i++) {
    sctp_sendmsg(sd, message, sizeof(message), 
                 (struct sockaddr *)&address, sizeof(address),
                 htonl(PPID),
                 0, /* flags */
                 0, /* stream no */
                 0, /* time to live */
                 0); /* context */
  } 

}

static void settings_init(void) {
  settings.host = "127.0.0.1";
  settings.port = 4242;
  settings.message = "hello";
  settings.count = 1;
  settings.no_of_threads = 1;
  settings.bufsize = 4096;
  settings.verbose= false;
}

void echoClient(char *host, int port, char *message, long count, int number_of_threads) {

  int rc;
  messageSenderData_t data[number_of_threads];
  pthread_t pth[number_of_threads];

  for(int i = 0; i < number_of_threads; i++) {
    //data = (messageSenderData_t*)malloc(sizeof(messageSenderData_));
    data[i].host = strdup(host);
    data[i].port = port;
    data[i].message = strdup(message);
    data[i].count = count;
    data[i].thread_id = i;

    if((rc = pthread_create(&pth[i], NULL, messageSender, &data[i]))) {
      perror("failure creating thread");
      continue;
    }
  }
  /* wait for every htread to finish */
  for(int i = 0; i < number_of_threads; i++) {
    pthread_join(pth[i], NULL);
  }
}

static void usage(void) {
  printf("-i <ip address/hostname>   server to connect to (default: localhost)\n"
      "-p <port number>              port number to connect to (default: 4242)\n"
      "-m <message>                  message to send (default: hello)\n"
      "-c <number of messages>      (default: 1)\n"
      "-n <number of threads>       (default: 1)\n" 
      );
  return;
}

int main(int argc, char **argv) {

  int c;

  settings_init();

  while (-1 != (c = getopt(argc, argv,
          "h:"
          "i:"
          "p:"
          "m:"
          "c:"
          "n:"
          "b:"
          "s"
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
      case 'c':
        settings.count = atol(optarg);
        break;
      case 'n':
        settings.no_of_threads= atoi(optarg);
        break;
      case 'b':
        settings.bufsize = atoi(optarg);
        break;
      case 's':
        settings.sctp = true;
        break;
      case 'v':
        settings.verbose = true;
        break;
      default:
        fprintf(stderr, "Illegal argument \"%c\"\n", c);
        exit(EXIT_FAILURE);
    }
  }
  echoClient(settings.host, settings.port, settings.message, settings.count, settings.no_of_threads);
}

