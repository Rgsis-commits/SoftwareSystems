/*
============================================================================
Name : 20.c
Author : Rishav Chandel
Description : Write two programs so that both can communicate by FIFO -Use one way communication
Date: 11th Oct, 2023.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

void main(){
    int fifostatus;
    int fd;
    char *fifoFile = "./myfifo";
    char readbytes;
    char data[100];
    
    fd = open(fifoFile, O_RDONLY);

    if(fd == -1)
        perror("Error while opening the FIFO file!");
    else{
        readbytes = read(fd,data,100);
        if(readbytes == -1)
            perror("Error while reading from the FIFO file!");
        else{
            printf("Bytes Read: %d\n", readbytes);
            printf("Data from the FIFO file- %s\n", data);
        }
        close(fd);
    }
}