/*
============================================================================
Name : 16.c
Author : Rishav Chandel
Description : Write a program to send and receive data from parent to child 
              vice versa. Use two way communication.
Date: 10th Oct, 2023.
============================================================================
*/

#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>

int main(){
    int fd1[2], fd2[2];
    if((pipe(fd1) == -1) || (pipe(fd2) == -1)){
        perror("Error in defining the pipe!");
        return -1;
    }
    char buf1[80]; //for read from parent and write to child
    char buf2[80]; // for read from child and write to parent
    if(fork()){
        close(fd1[1]);
        close(fd2[0]);
        printf("Enter message from parent: ");
        scanf("%[^\n]",buf1);
        write(fd2[1],buf1,sizeof(buf1));
        read(fd1[0],buf2,sizeof(buf2));
        printf("Message from child: %s\n",buf2);
    }
    else{
        close(fd2[1]);
        close(fd1[0]);
        read(fd2[0],buf1,sizeof(buf1));
        printf("Message from parent: %s\n",buf1);
        printf("Enter message from child: ");
        scanf("%[^\n]",buf2);
        write(fd1[1],buf2,sizeof(buf2));
    }
    wait(0);

    return 0;
}