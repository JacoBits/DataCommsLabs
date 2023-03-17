#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char* argv[]){

    struct sockaddr_in sin;
    char buffer[14]; //buffer for receiving the message
    int buffer_length; //length of the buffer

    // I tried to add a timestamp to each message that the server received, but it wasn't working :(
    //char timeStamp[27];
    //time_t ticks;

    printf("%s\n", "Waiting for client");
    int connectfd = 0;
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); //creates a socket in sockfd, instructs it to use TCP

    //memset statements for setting the memory of the buffers and sockaddr_in to 0
    memset(&sin, '0', sizeof(sin));
    memset(buffer, '0', sizeof(buffer));
    //memset(timeStamp, '0', sizeof(timeStamp));

    //setup for the sockaddr_in structure
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = htonl(INADDR_ANY);

    //sets the port to 6969
    sin.sin_port = htons(6969);
    
    //binds the socket
    bind(sockfd, (struct sockaddr*)&sin, sizeof(sin));

    //listens for the client
    listen(sockfd, 10);

    //accepts the client connection
    connectfd = accept(sockfd, (struct sockaddr*)NULL, NULL);

    //reads a message from the server, and adds a null terminator to it
    buffer_length = recv(connectfd, buffer, sizeof(buffer), 0);
    buffer[buffer_length] = '\0';
    //prints the message received from the client
    printf("Client: %s\n", buffer);
        
    //creates a timestamp of when the message was received (sadly this didn't work)
    //ticks = time(NULL);
    //strftime(timeStamp, sizeof(timeStamp), "[%c] ", localtime(&ticks));
    //snprintf(buffer, sizeof(buffer),"%s%s\n", timeStamp, buffer);
    //buffer[sizeof(buffer) - 1] = '\0';

    //sends the message to back to the client
    buffer_length = send(connectfd, buffer, strlen(buffer), 0);
    
    //sends "Bye" to the client
    buffer_length = send(connectfd, "\nBye", strlen("\nBye"), 0);

    //closes the socket and exits the while loop
    close(connectfd);

    return 0;

}