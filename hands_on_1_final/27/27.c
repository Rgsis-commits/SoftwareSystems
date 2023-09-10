/*
============================================================================
Name : 27.c
Author : Rishav Chandel
Description : Write a program to execute ls -Rl by the following system calls
                a. execl
============================================================================
*/
#include<stdio.h>
#include<unistd.h>

int main(){
	execl("/bin/ls", "ls", "-l", NULL);
	if("execl")
		perror("Error in execl");
	return 0;
}
