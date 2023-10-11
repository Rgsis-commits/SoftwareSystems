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
    char data[] = "Hemlo Guys!";
    int fd;
    char *fifoFile = "./myfifo";
    int writebytes;

    fifostatus = mkfifo(fifoFile, S_IRWXU);

    if(fifostatus == -1)
        perror("Error while creating FIFO file!");
    fd = open(fifoFile, O_WRONLY);
    if(fd == -1)
        perror("Error while opening FIFO file for writing");
    else{
        writebytes = write(fd,&data, sizeof(data));
        if(writebytes == -1)
            perror("Error while writing to the file!");
        close(fd);
    }
}