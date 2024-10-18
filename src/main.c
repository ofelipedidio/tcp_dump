#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> 

char buf[4096];

int main(int argc, char **argv) {
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli; 

    // socket create and verification 
    sockfd = socket(AF_INET, SOCK_STREAM, 0); 
    if (sockfd == -1) { 
        printf("ERROR: Socket creation failed\n"); 
        exit(1);
    }
    fprintf(stderr, "[INFO] Socket created\n"); 

    // assign IP, PORT 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(8080); 

    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr))) != 0) { 
        fprintf(stderr, "ERROR: Socket bind failed...\n"); 
        close(sockfd);
        exit(1);
    } 
    fprintf(stderr, "[INFO] Binded successful\n"); 

    // Now server is ready to listen and verification 
    if (listen(sockfd, 5) != 0) { 
        fprintf(stderr, "ERROR: Listen failed...\n"); 
        close(sockfd);
        exit(1);
    } 
    fprintf(stderr, "[INFO] listening...\n"); 
    len = sizeof(cli); 

    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (struct sockaddr*)&cli, &len); 
    if (connfd < 0) { 
        fprintf(stderr, "ERROR: Accept failed\n"); 
        close(sockfd);
        exit(1);
    } 
    fprintf(stderr, "[INFO] Accepted\n"); 

    ssize_t n;
    ssize_t res;
    while (1) {
        n = read(connfd, &buf, 4096);
        if (n == 0) {
            break;
        } else if (n < 0) {
            fprintf(stderr, "ERROR: Failed to read from socket\n");
            close(sockfd);
            exit(1);
        }
        res = write(0, buf, n);
        if (res < 0) {
            fprintf(stderr, "ERROR: Failed to write to stdout\n");
            close(sockfd);
            exit(1);
        }
    }

    // After chatting close the socket 
    close(sockfd);

    return 0;
}
