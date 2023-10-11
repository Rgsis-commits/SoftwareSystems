/*
============================================================================
Name : 22.c
Author : Rishav Chandel
Description : Write a program to wait for data to be written into FIFO 
              within 10 seconds, use select system call with FIFO.
Date: 11th Oct, 2023.
============================================================================
*/

#include<sys/stat.h>
#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/time.h>

#include "./22header.h"

void main(){
    int fd, readbytes;
    char data[100];

    fd_set readSet;
    struct timeval timer;
    int selectStatus;

    init();

    fd = open(fifofile, O_NONBLOCK | O_RDONLY);
    if(fd == -1){
        perror("Error while opening FIFO file!");
        _exit(0);
    }

    FD_ZERO(&readSet);
    FD_SET(fd, &readSet);

    timer.tv_sec = 10;
    timer.tv_usec = 0;

    selectStatus = select(fd+1, &readSet, NULL, NULL, &timer);
    if(selectStatus == -1)
        perror("Error while calling for select!");
    else if(selectStatus){
        readbytes = read(fd, &data, 100);

        if(readbytes == -1)
            perror("Error in reading from FIFO");
        else
            printf("Data received: %s\n", data);
    }
    else
        printf("No data sent for 10 seconds!\n");
    close(fd);
}