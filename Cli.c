#include <stdio.h> // For printf
#include <string.h> // For bzero
#include <stdlib.h> // For exit

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "config.h" // Including my own config file.

#define TRUE 1        // 1 means infinite loop
#define BUFF_SIZE 255 // buffer size

char *message = "HTTP/1.1 200 OK\r\n\r\nHello From Server!";

void clr(char *, int);

int main(int argc, char *argv[]) {
    if(argc < 2) {
        printf("ERROR: usage: %s port\n", argv[0]);
    }
    int port_number = atoi(argv[1]);
    int fd = socket(AF_INET, SOCK_STREAM, 0); // get the file descriptor
    if(fd < 0) {
        printf("ERROR: Failed to create the socket\n");
        exit(1);
    }
    else {
        printf("INFO: Created socket\n");
    }
    struct sockaddr_in server_address;
    // Need to fill the address values!
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(port_number);
    int status = bind(fd, (struct sockaddr *) &server_address, sizeof(server_address));
    if(status < 0) {
        printf("ERROR: Failed to bind to the port %s\n",argv[1]);
        exit(1);
    }
    else {
        printf("INFO: Bind the port: %s\n",argv[1]);
    }
    status = listen(fd, 1); // set backlog to 1 and reusing file descriptor fd and status interger
    if(status < 0) {
        printf("ERROR: Listen failed\n");
        exit(1);
    }
    else {
        printf("INFO: Listening on the given port: %s\n",argv[1]);
    }
    struct sockaddr_in client_address;
    socklen_t client_len;
    client_len = sizeof(client_address);
    char buffer[BUFF_SIZE];
    int bytes = 0;
    clr(buffer, BUFF_SIZE);
    printf("INFO: build time : %s\n", Build_Time);
restart:
    printf("Waiting for new connection.....\n");
    int nfd = accept(fd, (struct sockaddr *) &client_address, &client_len); // create a new socket file descriptor to hold the new connection and pass the socket file descriptor
    if(nfd < 0) {
        printf("ERROR: Accept failed\n");
        if(nfd != NULL) close(nfd);
        goto restart;
    }
    else {
        printf("INFO: Established [accepted] a new connection to a client\n");
    }
    while(TRUE) {
        clr(buffer, BUFF_SIZE); // clear the buffer
        bytes = read(nfd, buffer, BUFF_SIZE);
        if(bytes < 0) {
            printf("ERROR: cannot read from the client\n");
            close(nfd);
            goto restart;
        }
        else {
            printf("INFO: Client: ");
        }
        printf("%s", buffer);
        if(buffer[BUFF_SIZE - 1] == '\0') {
            clr(buffer, BUFF_SIZE);
            printf("INFO: preparing to send response, dected request end\n");
            // To change the message sent, change it above.
            sprintf(buffer, message);
            printf("INFO: buffer length is : %d\n", strlen(buffer));
            bytes = write(nfd, buffer, strlen(buffer));
            if(bytes < 0) {
                printf("ERROR: cannot write to the client\n");
                close(nfd);
                goto restart;
            }
            else if(bytes == strlen(buffer)) {
                printf("INFO: successfully sent the response!\n");
                //sleep(5);
                printf("INFO: resuming to reset connection\n");
                close(nfd);
                goto restart;
            }
            else {
                printf("ERROR: Sent bytes not equal to %d something is fishy!\n", strlen(buffer));
                close(nfd);
                goto restart;
                //continue;
            }
        }
        clr(buffer, BUFF_SIZE);
        printf("INFO: getting remaining request\n");
    }
    close(fd);
    return 0;
}
void clr(char *ptr, int len) {
    bzero(ptr, len);
}
