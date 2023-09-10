/*
============================================================================
Name : 7.c
Author : Rishav Chandel
Description : Write a program to copy file1 into file2 ($cp file1 file2).
============================================================================
*/
#include<fcntl.h>
#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Enter the filenames in arguments.");
		return -1;
	}
	int fd1 = open(argv[1], O_RDONLY, 0777);
	int fd2 = open(argv[2], O_WRONLY|O_CREAT, 0777);
	if(fd1 == -1 || fd2 == -1){
                perror("Error in opening file descriptor");
                return -1;
        }
	while(1){
		char buf;
		int readbytes = read(fd1,&buf,1);
		if(readbytes == 0)
			break;
		int writebytes = write(fd2,&buf,1);
	}
	int fd1_close = close(fd1);
	int fd2_close = close(fd2);
	if(fd1_close == -1 || fd2_close == -1){
		perror("Error in closing file descriptor");
		return -1;
	}
	return 0;
}
