/*
============================================================================
Name : 14.c
Author : Rishav Chandel
Description : Write a simple program to create a pipe, write to the pipe, 
              read from pipe and display on the monitor.
Date: 10th Oct, 2023.
============================================================================
*/
#include<stdio.h>
#include<unistd.h>


int main(){
    int pipefd[2];
    if(pipe(pipefd) == -1){
        perror("Error in declaring pipe!");
        return -1;
    } 
    char buf[80];
    int writebytes = write(pipefd[1], "Welcome\n", 8);
    read(pipefd[0],buf,writebytes);
    printf("From pipe: %s",buf);
    
    return 0;
}