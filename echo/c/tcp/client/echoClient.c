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
#include "../common.h"

struct settings settings;

typedef struct messageSender_data {
  char *host;
  int port;
  char *message;
  long count;
  int thread_id;
} messageSenderData_t;

void *messageSender(void *arg) {

  messageSenderData_t *data = (messageSenderData_t *) arg;

  struct sockaddr_in serverAddress;
  struct audit audit;
  audit.success = 0;
  audit.failure = 0;

  /* keep data local to thread */
  char *message = strdup(data->message);
  int count = data->count;

  bzero((void *)&serverAddress, sizeof(serverAddress));
  serverAddress.sin_family = AF_INET; 
  serverAddress.sin_port = htons(data->port);
  serverAddress.sin_addr.s_addr = inet_addr(data->host);

  int sd;
  if(settings.sctp) {
    sd = socket(PF_INET, SOCK_STREAM, IPPROTO_SCTP);
  } else {
    sd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  }

  if(-1 == connect(sd, 
        (struct sockaddr *) &serverAddress, 
        sizeof(serverAddress))) {
    perror("connection failure");
    close(sd);
    pthread_exit(NULL);
  }

  if(settings.sctp && settings.verbose) {
    struct sockaddr_in *paddrs[10];
    /*Get Peer Addresses*/
    int addr_count = sctp_getpaddrs(sd, 0, (struct sockaddr**)paddrs);
    printf("\nPeer addresses: %d\n", addr_count);

    /*Print Out Addresses*/
    for(int i = 0; i < addr_count; i++)
      printf("Address %d: %s:%d\n", 
             i +1, 
             inet_ntoa((*paddrs)[i].sin_addr), 
             (*paddrs)[i].sin_port);
    sctp_freepaddrs((struct sockaddr*)*paddrs);
  }


  int flag = 1;
  if(settings.sctp) {
    setsockopt(sd, IPPROTO_SCTP, SCTP_NODELAY, (char*)&flag, sizeof(flag));
  } else {
    setsockopt(sd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));
  }

  size_t length;
  uint32_t nlength, nid, ack_id, ack_nid;
  ssize_t numbytes;

  printf("thread %d start sending %d messages\n", data->thread_id, count);

  for(uint32_t id = 0; id < count; id++) {

    if(settings.verbose) printf("sending message %d\n", id);

    // send size of message
    length = strlen(message);
    nlength = htonl(length);

    if((numbytes = send(sd, &nlength, 4, 0)) == -1) {
      perror("send message size failure");
      audit.failure++;
      break;
    } 

    // send message id
    nid = htonl(id);
    if((numbytes = send(sd, &nid, 4, 0)) == -1) {
      perror("send message id failure");
      audit.failure++;
      break;
    }

    // send message
    if((numbytes = send(sd, message, length, 0)) == -1) {
      perror("send message failure");
      audit.failure++;
      break;
    }

    /* get response (message id) */
    if((numbytes = recv(sd, &ack_nid, 4, MSG_WAITALL)) == -1) {
      perror("recv id failure");
      audit.failure++;
      break;
    }

    ack_id = ntohl(ack_nid);
    if(ack_id != id) {
      printf("ack_id: %d != id: %d\n", ack_id, id);
      break;
    }

    audit.success++;
  }
 

  /* close the message stream by sending a zero length message */
  length = 0;
  nlength = htonl(length);
  if((numbytes = send(sd, &nlength, 4, 0)) == -1) {
      perror("send message size failure");
      close(sd);
      pthread_exit(NULL);
  }

  close(sd);

  printf("%lu messages send succesful by thread %d\n", audit.success, data->thread_id); 
  printf("%lu messages failed\n", audit.failure); 

  pthread_exit(NULL);
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
      "-s                           SCTP (default TCP)\n" 
      );
  return;
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

int main(int argc, char **argv) {
 
  int c;

  settings_init();

  while (-1 != (c = getopt(argc, argv,
          "h"
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


