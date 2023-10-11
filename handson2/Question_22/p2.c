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
    int fd, writebytes;
    char data[100];

    fd = open(fifofile, O_NONBLOCK | O_WRONLY);

    if(fd == -1){
        perror("Error while opening FIFO file!");
        _exit(0);
    }
    scanf("%[^\n]",data);
    writebytes = write(fd, data, 100);
    
    if(writebytes == -1)
        perror("Error while writing to FIFO!");

    close(fd);
}