/*
============================================================================
Name : 1b.c
Author : Rishav Chandel
Description : Create the following types of a files using system call
 b. hardlink (link system call)
============================================================================
*/
#include<stdio.h>
#include<unistd.h>

int main(int argc, char* argv[]){
	char* filename;
	char* hardlink;
	if(argc != 3){
		printf("Enter the filepath and hardlink path as arguments.\n");
		return -1;
	}
	filename = argv[1];
	hardlink = argv[2];
	int status;
	status = link(filename, hardlink);
	if(status == -1){
		perror("Error in creating hardlink.");
	}
	else{
		printf("Hardlink created successfully.\n");
	}
	return 0;
}

