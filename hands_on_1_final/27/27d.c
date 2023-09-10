/*
============================================================================
Name : 27d.c
Author : Rishav Chandel
Description : Write a program to execute ls -Rl by the following system calls
                a. execv
============================================================================
*/
#include<stdio.h>
#include<unistd.h>

int main(){
	char *args[] = {"ls", "-l", NULL};
        execv("/bin/ls", args);
        if("execlv")
                perror("Error in execlv");
        return 0;
}  
