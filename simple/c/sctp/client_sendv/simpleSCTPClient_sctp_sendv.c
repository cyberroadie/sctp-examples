#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/sctp.h>

#define RECVBUFSIZE 2048


int main() {
    char *message = "hello";

    int sd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    if(sd == -1) {
        perror("failure opening socket");
        exit(EXIT_FAILURE);
    }

    struct sctp_initmsg initmsg = {0};
    initmsg.sinit_num_ostreams = 1;
    int s = setsockopt(sd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg));
    if(s == -1) {
        perror("error setting socket options");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in address; // TODO replace with helper function
    bzero((void*)&address, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_port = htons(4242);
    char *host = "127.0.0.1";
    address.sin_addr.s_addr = inet_addr(host);
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
    sinfo.snd_ppid = htonl(424242);
    //sinfo.snd_flags = SCTP_UNORDERED;

    // receiving variables
    struct iovec echoiov[1];
    char buf[RECVBUFSIZE];
    echoiov->iov_base = buf;
    echoiov->iov_len = RECVBUFSIZE;

    struct sockaddr_in from;
    socklen_t *fromlen = NULL, infolen;
    int flags = 0;
    unsigned int infotype = 0;
    struct sctp_rcvinfo rinfo;
    bzero(&rinfo, sizeof(struct sctp_rcvinfo));
    infolen = sizeof(rinfo);

    struct sctp_initmsg initMsg; // TODO Replace with helper function
    bzero((void *)&initMsg, sizeof(initMsg));
    initMsg.sinit_num_ostreams = 100;
    initMsg.sinit_max_instreams = 0;
    initMsg.sinit_max_attempts = 0;
    initMsg.sinit_max_init_timeo = 0;

    setsockopt(sd, IPPROTO_SCTP, SCTP_INITMSG, &initMsg, sizeof(initMsg));


    int n = sctp_sendv(sd,
                       &iov,
                       1,
                       (struct sockaddr *) &address,
                       1,
                       (void *) &sinfo,
                       sizeof(struct sctp_sndinfo),
                       SCTP_SENDV_SNDINFO,
                       0);

    if(n == -1) {
        perror("failure sending message");
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

