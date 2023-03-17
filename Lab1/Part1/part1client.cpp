#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char* argv[]){
    int sockfd = 0; 
    int len = 0;
    struct sockaddr_in sin;
    char sendMsg[] = "Hello Fordham"; //message to send to the server
    char receiver[5] = {'\0'}; //receives the last message from the server

    //asks user to input ip
    if(argc != 2){
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }

    //creates socket, and error checking
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        printf("ERROR: could not create socket");
        return 1;
    }

    //memset for the sockaddr_in struct
    memset(&sin, '0', sizeof(sin));

    sin.sin_family = AF_INET;
    //sets the port to 6969
    sin.sin_port = htons(6969);


    //sets the ip address to the one input by the user
    if(inet_pton(AF_INET, argv[1], &sin.sin_addr.s_addr)<=0)
    {
        printf("\n inet_pton error occured\n");
        return 1;
    }

    //connects to the server socket
    if(connect(sockfd, (struct sockaddr*)&sin, sizeof(sin)) < 0){
       printf("\n ERROR: Connect Failed \n");
       return 1;
    }

    printf("Connection successful\n");

    //sends the message and receives it back from the server, then prints it
    len = send(sockfd, sendMsg, strlen(sendMsg), 0);
    len = recv(sockfd, sendMsg, sizeof(sendMsg), 0);
    printf("Server: %s", sendMsg);
        
    //receives a second message from the server into receiver, and prints it
    len = recv(sockfd, receiver, sizeof(receiver) - 1, 0);
    receiver[len] = '\0';
    printf("Server: %s\n", receiver);

    //closes the socket
    close(sockfd);
    return 0;

}