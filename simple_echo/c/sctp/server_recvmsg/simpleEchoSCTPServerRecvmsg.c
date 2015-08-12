#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/sctp.h>
#include <strings.h>
#include "myutil.h"

int main() {

    int sd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    if(sd == -1) {
        perror("failure opening socket");
        exit(EXIT_FAILURE);
    }

    // listen on 0.0.0.0:4242, any NIC and port 4242
    struct sockaddr_in serverAddress;
    createAddress(INADDR_ANY, "4242", &serverAddress, "sctp");

    int rb = bind(sd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if(rb == -1) {
        perror("failed to bind");
        close(sd);
        exit(EXIT_FAILURE);
    }

    int lr = listen(sd, 5);
    if(lr == -1) {
        perror("listen failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    struct sctp_initmsg initmsg;
    createInitMsg(&initmsg, 10, 10, 0, 0);

    int s = setsockopt(sd, IPPROTO_SCTP, SCTP_INITMSG, &initmsg, sizeof(initmsg));
    if(s == -1) {
        perror("error setting socket options");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // indicate we want information returned
    int on = 1;
    if (setsockopt(sd, IPPROTO_SCTP, SCTP_RECVRCVINFO, &on, sizeof(on)) < 0) {
        perror("setsockopt SCTP_RECVRCVINFO");
        exit(EXIT_FAILURE);
    }

    int rl = listen(sd, 1);
    if(rl < -1) {
        perror("listen failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    char message[RECVBUFSIZE];

    struct msghdr msghdr;
    createMessageHdrRcv(&msghdr,
                           (void *) &message,
                           RECVBUFSIZE
    );

    for(;;) {
        bzero(message, sizeof(message));
        size_t length = recvmsg(sd, &msghdr, 0);
        if(length < 0) {
            perror("error receiving message");
            close(sd);
            exit(EXIT_FAILURE);
        }

        struct cmsghdr *cmsg;
        struct sctp_rcvinfo *rinfo;

        if(msghdr.msg_controllen > 0) {
            cmsg = CMSG_FIRSTHDR(&msghdr);
            if (cmsg->cmsg_type == SCTP_RCVINFO) {
                rinfo = (struct sctp_rcvinfo *) CMSG_DATA(cmsg);
                printf("[ Receive echo (%u bytes): stream = %hu, "
                               "flags = %hx, ppid = %u ]\n", length,
                       rinfo->rcv_sid, rinfo->rcv_flags,
                       rinfo->rcv_ppid);
            }
        }

        message[length] = '\0';
        printf("message received: %s\n", message);

    }

}
