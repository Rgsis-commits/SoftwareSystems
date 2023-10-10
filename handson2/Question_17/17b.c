/*
============================================================================
Name : 17b.c
Author : Rishav Chandel
Description : Write a program to execute ls -l | wc.
                b. use dup2
Date: 10th Oct, 2023.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
    int fd[2];
    if(pipe(fd) == -1){
        perror("Error in defining the pipe!");
        return -1;
    }
    if(fork()){
        close(fd[0]);
        dup2(fd[1],1); // Duplicate fd[1] to lowest file descriptor(fd) available value = 1
        execlp("ls", "ls", "-l", (char*) NULL);
        // execlp will write the output of the "ls -l" to fd with valud = 1
    }
    else{
        close(fd[1]); 
        dup2(fd[0],0); // Duplicate fd[0] to the lowest fd available value = 0
        execlp("wc", "wc", (char*) NULL);
        /* execlp() will read from the fd with value = 0 (read-end of pipe) as input
        to "wc" command and show output to fd with the value = 1 i.e. STDOUT
        */    
    }
    wait(0);
    return 0;
}