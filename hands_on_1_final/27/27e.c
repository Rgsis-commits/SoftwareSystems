/*
============================================================================
Name : 27e.c
Author : Rishav Chandel
Description : Write a program to execute ls -Rl by the following system calls
                a. execvp
============================================================================
*/
#include<stdio.h>
#include<unistd.h>

int main(){
        char *args[] = {"ls", "-l", NULL};
        execv("/bin/ls", args);
        perror("Error in execvp");
        return 0;
}
