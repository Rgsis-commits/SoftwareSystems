#include <stdio.h> // Import for `printf` & `perror` functions
#include <errno.h> // Import for `errno` variable

#include <fcntl.h>      // Import for `fcntl` functions
#include <unistd.h>     // Import for `fork`, `fcntl`, `read`, `write`, `lseek, `_exit` functions
#include <sys/types.h>  // Import for `socket`, `bind`, `listen`, `accept`, `fork`, `lseek` functions
#include <sys/socket.h> // Import for `socket`, `bind`, `listen`, `accept` functions
#include <netinet/ip.h> // Import for `sockaddr_in` stucture

#include <string.h>  // Import for string functions
#include <stdbool.h> // Import for `bool` data type
#include <stdlib.h>  // Import for `atoi` function

#include "./Handler/admin_handler.h"
#include "./Handler/faculty_handler.h"
#include "./Handler/student_handler.h"


void connection_handler(int connectFD){
    printf("Connection has been established.");

    char readBuffer[1000], writeBuffer[1000];
    ssize_t readBytes, writeBytes;
    int userChoice;

    char greet[] = "Welcome to IIITB Academia\n1) Admin\n2) Student\n3) Faculty\nEnter your choice:";
    writeBytes = write(connectFD, greet,sizeof(greet));
    if(writeBytes == -1){
        perror("Error while sending first prompt to the user!");
    }
    else{
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connectFD, readBuffer, sizeof(readBuffer));
        if(readBytes == -1)
            perror("Error while reading from the client!");
        else if(readBytes == 0)
            printf("No data was sent by the client");
        else{
            userChoice = atoi(readBuffer);
            switch (userChoice)
            {
                case 1:
                    // Admin
                    admin_handler(connectFD);
                    break;
                    
                case 2:
                    // Student
                    student_handler(connectFD);
                    break;   
                case 3: 
                    // Faculty
                    faculty_handler(connectFD);
                    break;
                default:
                    //Exit
                    printf("Invalid Input!");
                    break;

            }
        }
    }
    printf("Terminating connection to client!\n");
}


int main(){
    int soc, socBindStatus, socListenStatus, connectionFD;
    struct sockaddr_in servAddr, clientAddr; 
    soc = socket(AF_INET, SOCK_STREAM, 0);
    if(soc == -1){
        perror("Error while creating server socket!");
        _exit(0);
    } 

    servAddr.sin_family = AF_INET;                // IPv4
    servAddr.sin_port = htons(9090);              // Server will listen to port 9090
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY); // Binds the socket to all interfaces

    socBindStatus = bind(soc, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if(socBindStatus == -1){
        perror("Error while binding the socket to server!");
        _exit(0);
    }

    socListenStatus = listen(soc, 10); // server can listen to the maximum of 10 clients
    if(socListenStatus == -1){
        perror("Error while listening for connections on the server socket!");
        close(soc);
        _exit(0);
    }

    int clientSize;
    while(1){
        clientSize = (int)sizeof(clientAddr);
        connectionFD = accept(soc, (struct sockaddr *)&clientAddr, &clientSize); // accepting the client's scoket address
        if(connectionFD == -1){
            perror("Error while connecting to client!");
            close(soc);
        }
        else{
            if(!fork()){
                // Child will enter this branch
                connection_handler(connectionFD);
                close(connectionFD);
                _exit(0);
            }
        }
    }
}