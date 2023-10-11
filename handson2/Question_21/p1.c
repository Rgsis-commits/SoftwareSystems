/*
============================================================================
Name : 21.c
Author : Rishav Chandel
Description : Write two programs so that both can communicate by FIFO -Use 
              two way communications.
Date: 11th Oct, 2023.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>

#include "./21header.h"

int main(){
    int readbytes, writebytes;
    int fd1, fd2;
    char data[100];

    init();

    fd1 = open(file1, O_WRONLY);
    if(fd1 == -1){
        perror("Error while opening FIFO file one!");
        return 1;
    }

    fd2 = open(file2, O_RDONLY);
    if(fd2 == -1){
        perror("Error while opening the FIFO second file!");
        close(fd1);
        return 1;
    }

    writebytes = write(fd1, "It's hello not hemlo!",22);

    if(writebytes == -1)
        perror("Error in writing the data!");
    close(fd1);

    readbytes = read(fd2, &data, 100);

    if(readbytes == -1){
        perror("Error while reading from FIFO file 2");
        close(fd2);
        return 1;
    }

    printf("Data from FIFO: %s\n", data);

    close(fd2);

    return 0;
}
