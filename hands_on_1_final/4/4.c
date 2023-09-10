/*
============================================================================
Name : 3.c
Author : Rishav Chandel
Description : Write a program to open an existing file with read write mode. Try O_EXCL flag also. 
============================================================================
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("You have given wrong number of arguments\n");
		return 0;
	}
	int fd_rdwr = open(argv[1],O_RDWR);
	if(fd_rdwr == -1){
		perror("Error in opening file descriptor.\n");
		return -1;
	}
	printf("File descriptor for reading and writing is %d \n",fd_rdwr);
	int fd_rdwr_close = close(fd_rdwr);
	if(fd_rdwr_close == -1){
                perror("Error in closing the file descriptor.\n");
                return 0;
        }
	return 0;
}
