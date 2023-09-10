/*
============================================================================
Name : 1a.c
Author : Rishav Chandel
Description : Create the following types of a files using system call
 a. soft link (symlink system call)
============================================================================
*/
#include<stdio.h>
#include<unistd.h>

int main(int argc, char* argv[]){
	char* filename;
	char* softlink;
	if(argc != 3){
		printf("Enter the file path and softlink path in the arguments");
		return -1;
	}
	filename = argv[1];
	softlink = argv[2];
	int status;
	status = symlink(filename,softlink);
	if(status == -1){
		perror("Error in creating softlink");
	}
	else{
		printf("Softlink created successfully.");
	}
	return 0;
}

