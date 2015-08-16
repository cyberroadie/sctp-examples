#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>
#include "myutil.h"
#include "simpleEchoTCPServer.h"

#define MSG_SIZE 6

int main() {

    // create socket
    int sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sd == -1) {
        perror("failed to create socket");
        exit(EXIT_FAILURE);
    }

    // server address and port to listen on (0.0.0.0) (4242)
    struct sockaddr_in serverAddress;
    createAddress(INADDR_ANY, "4242", &serverAddress, "tcp");

    // bind socket to address
    int rb = bind(sd, (struct sockaddr *)&serverAddress, sizeof(serverAddress));
    if(rb == -1) {
        perror("failed to bind");
        close(sd);
        exit(EXIT_FAILURE);
    }

    // listen to socket
    int lr = listen(sd, 5);
    if(lr == -1) {
        perror("listen failed");
        close(sd);
        exit(EXIT_FAILURE);
    }

    char msg[MSG_SIZE];
    char *rmsg = "thanks";
    struct sockaddr_in clientAddress;
    socklen_t len = sizeof(clientAddress);

    for(;;) {
        // accept connections
        int cd = accept(sd, (struct sockaddr *)&clientAddress, &len);
        if(cd < 0) {
            perror("fail on accept");
            exit(EXIT_FAILURE);
        }
        printf("connection accepted");

        bzero(msg, MSG_SIZE);
        // read from client socket
        int n = read(cd, msg, MSG_SIZE);
        if(n <  0) {
            perror("error reading");
            close(cd);
            continue;
        }

        printf("message received: %s", msg);

        // write return message
        int m = write(cd, rmsg, sizeof(rmsg));
        if(m < 0) {
            perror("error writing");
        }

        // close client socket
        close(cd);

    }

}