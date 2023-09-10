/*
============================================================================
Name : 27b.c
Author : Rishav Chandel
Description : Write a program to execute ls -Rl by the following system calls
                b. execlp
============================================================================
*/
#include<stdio.h>
#include<unistd.h>

int main(){
        execlp("ls", "ls", "-l", NULL);
        if("execl")
                perror("Error in execl");
        return 0;
}

