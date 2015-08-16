#include <stdio.h>
#include <stdlib.h>
#include <netinet/sctp.h>
#include <unistd.h>
#include "myutil.h"

int main() {
    int sd = socket(AF_INET, SOCK_SEQPACKET, IPPROTO_SCTP);
    if(sd == -1) {
        perror("failure opening socket");
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

    struct sockaddr_in peer;
    createAddress("127.0.0.1", "4242", &peer, "sctp");

    struct sctp_sndrcvinfo sinfo;
    createSndRcvInfo(&sinfo, 1, 0, 0, 0, 0);

    char *message = "hello";

    struct msghdr msghdr;
    createMessageHdrSndRcv(&msghdr,
                           &initmsg,
                           &sinfo,
                           (struct sockaddr *) &peer,
                           sizeof(peer),
                           (void *) message,
                           sizeof(message)
    );

    ssize_t n = sendmsg(sd, &msghdr, 0);
    if(n < 0) {
        perror("error sending message");
        close(sd);
        exit(EXIT_FAILURE);
    }

    close(sd);
    exit(EXIT_SUCCESS);
}
