#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/sctp.h>
#include <unistd.h>
#include <arpa/inet.h>

#define RECVBUFSIZE 10

int main() {

    int sd;

    sd = socket(PF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);

    if(sd == -1) {
        perror("failure opening socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddress;
    bzero((void *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
    serverAddress.sin_port = htons(4242);

    int rb = bind(sd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if(rb == -1){
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

    const int on = 1;
    setsockopt(sd, IPPROTO_SCTP, SCTP_LISTEN_FIX, &on, sizeof(int));

    int rl = listen(sd, 1);
    if(rl < 0) {
        perror("failed to listen for connection");
        close(sd);
        exit(EXIT_FAILURE);
    }

    struct iovec iov[1];
    char buf[RECVBUFSIZE];
    iov->iov_base = buf;
    iov->iov_len = RECVBUFSIZE;

    if (setsockopt(sd, IPPROTO_SCTP, SCTP_RECVRCVINFO, &on, sizeof(on)) < 0) {
        perror("setsockopt SCTP_RECVRCVINFO");
        exit(EXIT_FAILURE);
    }

    struct sctp_sndinfo sinfo;
    bzero(&sinfo, sizeof(struct sctp_sndinfo));


    for(;;) {
        struct sockaddr_in  client_addr;
        bzero((void*)&client_addr, sizeof(client_addr));
        socklen_t fromlen, infolen;
        int flags = 0;
        unsigned int infotype = 0;
        struct sctp_rcvinfo rinfo;
        bzero(&rinfo, sizeof(struct sctp_rcvinfo));
        infolen = sizeof(rinfo);
        fromlen = sizeof(client_addr);

        int length = sctp_recvv(sd,
                                iov,
                                1,
                                (struct sockaddr *) &client_addr,
                                &fromlen,
                                &rinfo,
                                &infolen,
                                &infotype,
                                &flags);

        if(length == -1) {
            perror("error receiving message: ");
            exit(EXIT_FAILURE);
        }

        buf[length] = '\0';
        printf("message received: %s\n", buf);

    }

    return EXIT_SUCCESS;
}
