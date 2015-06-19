#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h> /* optarg */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h> 
#include <netinet/sctp.h> 
#include <netinet/sctp_uio.h> 
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
  
  struct sctp_initmsg initmsg = {0};
  initmsg.sinit_num_ostreams = 4;
  int s = setsockopt(sd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg));
  if(s == -1) {
    perror("Error setting socket options");
    exit(EXIT_FAILURE);
  }

  struct sockaddr_in  address;
  bzero((void*)&address, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(data->port);
  address.sin_addr.s_addr = inet_addr(data->host);
  address.sin_len = (socklen_t) sizeof(struct sockaddr_in);

  // Structure containing data
  struct iovec iov;
  bzero(&iov, sizeof(iov));
  iov.iov_base = message;
  iov.iov_len = strlen(message);

  struct sctp_sndinfo sinfo;
  bzero(&sinfo, sizeof(struct sctp_sndinfo));

  // Send information structure
  sinfo.snd_sid = 1;
  sinfo.snd_ppid = 424242;

  char *cmsgbuf;
  int addrcnt = 1;
  cmsgbuf = malloc(CMSG_SPACE(sizeof(struct sctp_sndinfo)) +
                   CMSG_SPACE(sizeof(struct sctp_prinfo)) +
                   CMSG_SPACE(sizeof(struct sctp_authinfo)) +
                   addrcnt * CMSG_SPACE(sizeof(struct in6_addr)));

  struct msghdr msg;
  struct cmsghdr *cmsg;
  msg.msg_control = cmsgbuf;
  msg.msg_controllen = 0;
  cmsg = (struct cmsghdr *)cmsgbuf;

  cmsg->cmsg_level = IPPROTO_SCTP;
  cmsg->cmsg_type = SCTP_SNDINFO;
  cmsg->cmsg_len = CMSG_LEN(sizeof(struct sctp_sndinfo));
  memcpy(CMSG_DATA(cmsg), &sinfo, sizeof(struct sctp_sndinfo));
  msg.msg_controllen += CMSG_SPACE(sizeof(struct sctp_sndinfo));
  cmsg = (struct cmsghdr *)((caddr_t)cmsg + CMSG_SPACE(sizeof(struct sctp_sndinfo)));

  msg.msg_name = &address;
  msg.msg_namelen = (socklen_t) sizeof(struct sockaddr_in);

  msg.msg_iov = &iov;
  msg.msg_iovlen = 1;
  msg.msg_flags = 0;

  for(int i = 0; i < count; i++) {
    //int n = sendmsg(sd, &msg, 1);
    int n = sctp_sendv(sd, &iov, 1, (struct sockaddr *) &address, 1, (void *) &sinfo, sizeof(struct sctp_sndinfo), SCTP_SENDV_SNDINFO, 0);
    if(n == -1) {
      perror("failure sending message");
      exit(EXIT_FAILURE);
    }
    //sendmsg(sd, msg, 0);
    //sctp_sendv(int, const struct iovec *, int, struct sockaddr *, int, void *, socklen_t, unsigned int, int))
    //sctp_sendv __P((int, const struct iovec *, int, struct sockaddr *, int, void *, socklen_t, unsigned int, int));
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

