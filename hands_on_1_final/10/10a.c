/*
============================================================================
Name : 10.c
Author : Rishav Chandel
Description : Write a program to open a file with read write mode, write 10 bytes, move the file pointer by 10
bytes (use lseek) and write again 10 bytes.
 a. check the return value of lseek
 b. open the file with od and check the empty spaces in between the data
============================================================================
*/
#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>

int main(){
	int fd = open("check.txt",O_RDWR|O_CREAT|O_TRUNC, 0777);
	if(fd == -1){
		perror("Error in opening file.");
		return -1;
	}
	char first[10] = "Hello_Life";
	char second[10] = "DigitalWld";
	int writebytes = write(fd,first,10);
	if(writebytes == -1){
		perror("Error in writing first 10 bytes.");
		return -1;
	}
	int status = lseek(fd, 10, SEEK_CUR);
	printf("Status of seek command: %d.\n",status);
	if(status == -1){
		perror("Error in moving seek");
		return -1;
	}
	writebytes = write(fd,second,10);
	if(writebytes == -1){
		perror("Error in writing second 10 bytes.");
		return -1;
	}
	int close_fd = close(fd);
	if(close_fd == -1){
		perror("Error in closing the file.");
		return -1;
	}
	execl("/usr/bin/od", "od", "-c", "check.txt", NULL);
	return -1;
}
