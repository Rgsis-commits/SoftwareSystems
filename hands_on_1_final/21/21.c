/*
============================================================================
Name : 21.c
Author : Rishav Chandel
Description : Write a program, call fork and print the parent and child process id.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>

int main(){
	if(!fork()){
		printf("The process ID of child is %d\n",getpid());
	}
	else{
		printf("The process ID of parent is %d\n", getpid());
	}
	return 0;
}
