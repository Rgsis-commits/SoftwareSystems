/*
============================================================================
Name : 15.c
Author : Rishav Chandel
Description : Write a simple program to send some data from parent to the 
              child process.
Date: 10th Oct, 2023.
============================================================================
*/
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
    char buf[80];
    int fd[2];
    if(pipe(fd) == -1){
        perror("Error in defining the pipe!");
        return -1;
    }

    if(fork()){
        close(fd[0]); // closing the read end of pipe for parent
        printf("Enter the message for the child: ");
        scanf("%[^\n]",buf); // Read till newline encountered
        write(fd[1],buf,sizeof(buf));
    }
    else{
        close(fd[1]);
        read(fd[0],buf,sizeof(buf));
        printf("Message from parent: %s\n", buf);
    }
    wait(0); // wait till all the children process have terminated
    return 0;
}