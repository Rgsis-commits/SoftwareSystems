/*
============================================================================
Name : 26.c
Author : Rishav Chandel
Description : Write a program to execute an executable program.
 a. use some executable program
 b. pass some input to an executable program. (for example execute an executable of $./a.out name)
============================================================================
*/
#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(int argc, char* argv[]){
        if(argc != 2){
                printf("There is error in passing the arguments.");
                return -1;
        }
        char *path = "./ticketreserve";
        execl(path, argv[1], NULL);
        perror("excel");
        return 0;

}

