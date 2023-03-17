#include <stdio.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

const int MAX_SIZE = 1024;
const int portNum = 6969;

int main(int argc, char* argv[]){
    int sockfd = 0; 
    int len = 0;
    struct sockaddr_in sin;
    char firstMsg[] = "Hello Fordham\n"; //first message to send to the server
    char secondMsg[] = "Hello CISC4615\n"; //second message to send to server
    char receiver[MAX_SIZE] = {0}; //receives the last message from the server

    std::string message; //string in which to hold the received messages temporarily
    std::vector<std::string> msgList; //vector to hold the messages received from the server

    //asks user to input ip
    if(argc != 2){
        printf("\n Usage: %s <ip of server> \n", argv[0]);
        return 1;
    }

    //creates socket, and error checking
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        perror("ERROR: could not create socket");
        return -1;
    }

    //memset for the sockaddr_in struct
    memset(&sin, '0', sizeof(sin));

    sin.sin_family = AF_INET;
    //sets the port number
    sin.sin_port = htons(portNum);


    //sets the ip address to the one input by the user
    if(inet_pton(AF_INET, argv[1], &sin.sin_addr.s_addr)<=0)
    {
        perror("\n inet_pton error occured\n");
        return -1;
    }

    //connects to the server socket
    if(connect(sockfd, (struct sockaddr*)&sin, sizeof(sin)) < 0){
       perror("\n ERROR: Connect Failed \n");
       return -1;
    }

    printf("Connection successful\n");
    while(true){
        
        //sends the first message, and error checks
        if((send(sockfd, firstMsg, sizeof(firstMsg), 0)) < 0){
            perror("Send error (first message)\n");
            return -1;
        }

        //receives the first message, and error checks
        if((len = recv(sockfd, receiver, MAX_SIZE, 0)) < 0){
            perror("Receive error (first message)\n");
            return -1;
        }

        //adds the null terminator to the received string
        receiver[len] = '\0';

        //converts the message received into an string type, and stores it into msgList
        for(int iter = 0; receiver[iter] != '\0'; iter++){
            message += receiver[iter];
        }
        msgList.push_back(message);
        message = "";

        //sends the second message, error checks
        if(send(sockfd, secondMsg, sizeof(secondMsg), 0) < 0){
            perror("Send error (second message)\n");
            return -1;
        }

        //sets the buffer to 0 (clears it), and then receives the second message
        memset(receiver, '0', len);
        if((len = recv(sockfd, receiver, MAX_SIZE, 0)) < 0){
            perror("Receive error (first message)\n");
            return -1;
        }

        receiver[len] = '\0';

        for(int iter = 0; receiver[iter] != '\0'; iter++){
            message += receiver[iter];
        }

        msgList.push_back(message);

        close(sockfd); //close socket
        break;
    }

    printf("Messages received from server:\n");
    for(int i = 0; i < msgList.size(); i++){
        std::cout << "Message "<< i + 1 << ": " << msgList[i];
    }

    
    return 0;

}