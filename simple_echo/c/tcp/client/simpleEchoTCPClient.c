#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <netinet/tcp.h>
#include <strings.h>
#include <arpa/inet.h>
#include "simpleEchoTCPClient.h"


int main() {

    // create socket
    int cd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(cd == -1) {
        perror("failure creating socket");
        exit(EXIT_FAILURE);
    }

    char *host = "127.0.0.1";

    // set server address
    struct sockaddr_in serverAddress;
    bzero((void *)&serverAddress, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(host);
    serverAddress.sin_port = htons(4242);

    // connect to server
    int co = connect(cd, (struct sockaddr *) &serverAddress, sizeof(serverAddress));

    if(co == -1) {
        perror("failure to connect");
        close(cd);
        exit(EXIT_FAILURE);
    }

//    int flag =  1;
//    setsockopt(cd, IPPROTO_TCP, TCP_NODELAY, (char*)&flag, sizeof(flag));

    char *msg = "hello";
    int length = strlen(msg);
    int n = send(cd, msg, length, 0);
    if(n <  0) {
        perror("error sending message");
        close(cd);
        exit(EXIT_FAILURE);
    }

    char rmsg[7];
    bzero(rmsg, sizeof(rmsg));

    // receive message
    int m = read(cd, rmsg, sizeof(rmsg));
    if(m < 0) {
        perror("error reading message");
        close(cd);
        exit(EXIT_FAILURE);
    }

    printf("message received: %s", rmsg);
    close(cd);

}