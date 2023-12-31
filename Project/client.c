#include <stdio.h>      // Import for `printf` & `perror` functions
#include <errno.h>      // Import for `errno` variable
#include <fcntl.h>      // Import for `fcntl` functions
#include <unistd.h>     // Import for `fork`, `fcntl`, `read`, `write`, `lseek, `_exit` functions
#include <sys/types.h>  // Import for `socket`, `bind`, `listen`, `connect`, `fork`, `lseek` functions
#include <sys/socket.h> // Import for `socket`, `bind`, `listen`, `connect` functions
#include <netinet/ip.h> // Import for `sockaddr_in` stucture
#include <string.h>     // Import for string functions

void clear_stdin() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void connection_handler(int socFD){
    char readBuffer[1000], writeBuffer[1000]; // To read or write to server
    int readBytes = 0, writeBytes = 0;            

    char tempBuffer[1000];
    do{
        bzero(readBuffer, sizeof(readBuffer));
        bzero(tempBuffer, sizeof(tempBuffer));
        readBytes = read(socFD, readBuffer, sizeof(readBuffer));
        if(readBytes == -1)
            perror("Error while reading from client socket!");
        else if(readBytes == 0)
            printf("No error received from server! Closing the connection to the server now!\n");
        else if(strchr(readBuffer, '^') != NULL){
            // skip read from client
            strncpy(tempBuffer, readBuffer, strlen(readBuffer) - 1);
            printf("%s\n", tempBuffer);
            writeBytes = write(socFD, "^", strlen("^"));
            if(writeBytes == -1){
                perror("Error while writing to client socket!");
                break;
            }
        }

        else if(strchr(readBuffer,'$') != NULL){
            // Server sent an error message and is now closing it's end of  the connection
            strncpy(tempBuffer, readBuffer, strlen(readBuffer)-1);
            printf("%s\n", tempBuffer);
            printf("Closing the connection to the server now!\n");
            break;
        }
        else{
            bzero(writeBuffer, sizeof(writeBuffer)); // Empty the write buffer
            if(strchr(readBuffer, '#') != NULL)
                // Taking input from readbuffer and printing it to terminal and waiting till the next \n 
                strcpy(writeBuffer, getpass(readBuffer));
            else{
                printf("%s\n", readBuffer);
                scanf("%[^\n]%*c",writeBuffer);
            }
            //bzero(readBuffer, sizeof(readBuffer));
            writeBytes = write(socFD, writeBuffer, strlen(writeBuffer));
            if(writeBytes == -1){
                perror("Error while writing to client socket!");
                printf("Closing the connection!");
                break;
            }
        }
    }while(readBytes > 0);
    
    close(socFD);

}

int main(){
    int soc, connectStatus;
    struct sockaddr_in servAddr;
    
    soc = socket(AF_INET, SOCK_STREAM, 0);
    if(soc == -1){
        perror("Error while creating server socket!");
        _exit(0);
    }

    servAddr.sin_family = AF_INET;                  // IPv4
    servAddr.sin_port = htons(9090);                // Server will listen to port 9090
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);   // Binds the socket to all interfaces

    connectStatus = connect(soc, (struct sockaddr *)&servAddr, sizeof(servAddr));
    if(connectStatus == -1){
        perror("Error while connecting to server!");
        close(soc);
        _exit(0);
    }

    connection_handler(soc);

    close(soc);
    return 0;
}
