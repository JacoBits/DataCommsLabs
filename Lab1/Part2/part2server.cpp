#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

const int MAX_SIZE = 1024;
const int portNum = 6969;

int main(int argc, char*argv[]){

    //structs for the server and client address
    struct sockaddr_in serv_addr;
    struct sockaddr_in client_addr;

    int sockfd = 0, listenfd = 0; //hold the sockets
    char buffer[MAX_SIZE]; //buffer
    int length = 0; //length of receive buffer

    //messages to send
    char Hello[] = "Hello CIS Students\n";
    char Bye[] = "Bye\n";

    //size of receive buffer
    socklen_t sin_size;

    //memset of serv_addr and buffer
    memset(&serv_addr, '0', sizeof(serv_addr));
    memset(buffer, '0', sizeof(buffer));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    //sets port number
    serv_addr.sin_port = htons(portNum);

    //error checking for socket() function
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket error!\n");
        return -1;
    }

    //error checking for bind() function
    if((bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(struct sockaddr))) < 0){
        perror("Bind error!\n");
        return -1;
    }

    //listen for client
    printf("Waiting for client...\n");
    listen(sockfd, 10);

    while(true){

        //error checking for accept() function
        sin_size = sizeof(struct sockaddr_in);
        if((listenfd = accept(sockfd, (struct sockaddr*)&client_addr, &sin_size)) < 0){
            perror("Accept error!\n");
            return -1;
        }

        //receives first message from the client, checks it was actually received
        if((length = recv(listenfd, buffer, MAX_SIZE, 0)) > 0){
            //print out the message
            buffer[length] = '\0';
            printf("Client says: ");
            for(int iter = 0; buffer[iter] != '\0'; iter++){
                printf("%c", buffer[iter]);
            }

            //sends the first message to the client, checks it was sent
            if((send(listenfd, Hello, sizeof(Hello), 0)) < 0){
                perror("Send error (first message)\n");
                return -1;
            }
            
            //receives second message, checks it was received, and prints it
            if((length = recv(listenfd, buffer, MAX_SIZE, 0)) > 0){
                buffer[length] = '\0';

                printf("Client says: ");
                for(int iter = 0; buffer[iter] != '\0'; iter++){
                    printf("%c", buffer[iter]);
                }
            }else{
                perror("Receive error (Second message)\n");
                return -1;
            }

            //sends second message, checks it was sent
            if((send(listenfd, Bye, sizeof(Bye), 0)) < 0){
                perror("Send error (second message)\n");
                return -1;
            }
        }else{
            perror("Receive error (first message)\n");
            return -1;
        }

        //close connection
        close(listenfd);
        break;
    }

    return 0;
}