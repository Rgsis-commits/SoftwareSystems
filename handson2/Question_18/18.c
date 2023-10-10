/*
============================================================================
Name : 18.c
Author : Rishav Chandel
Description : Write a program to find out total number of directories on the pwd.
              execute ls -l | grep ^d | wc ? Use only dup2.
Date: 10th Oct, 2023.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
    int fd1[2];
    int fd2[2];
    if((pipe(fd1) == -1) || (pipe(fd2) == -1)){
        perror("Error in defining the pipe!");
        return -1;
    }
    if(fork()){
        if(fork()){
            dup2(fd1[1],1); 
            close(fd1[0]);
            close(fd2[0]);
            close(fd2[1]);
            execlp("ls", "ls", "-l", (char*) NULL);
            
        }
        else{
            dup2(fd1[0],0);
            dup2(fd2[1],1); 
            close(fd1[1]);
            close(fd2[0]); 
            execlp("grep", "grep", "^d", (char*) NULL);   
        }
    }
    else{
        dup2(fd2[0],0); 
        close(fd1[1]);
        close(fd1[0]);
        close(fd2[1]); 
        execlp("wc", "wc", (char*) NULL);   
    }
    wait(0);
    return 0;
}