/*
============================================================================
Name : 17a.c
Author : Rishav Chandel
Description : Write a program to simulate online ticket reservation. Implement write lock
Write a program to open a file, store a ticket number and exit.
============================================================================
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main(){
	int fd = open("Ticket.txt", O_RDWR|O_CREAT);
	if(fd == -1){
                printf("Your system is useless it doesn't even know how to open a file descriptor");
                return -1;
        }
	int buffer = 10;
	write(fd,&buffer,sizeof(buffer));
	int fd_close = close(fd);
	if(fd_close == -1){
		printf("Your system is useless it doesn't even know how to close file descriptor");
		return -1;
	}
	return 0;
}
