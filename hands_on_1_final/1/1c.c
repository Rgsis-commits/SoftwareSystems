/*
============================================================================
Name : 1c.c
Author : Rishav Chandel
Description : Create the following types of a files using system call
c.  FIFO (mkfifo Library Function or mknod system call)
============================================================================
*/
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<stdio.h>
#include<unistd.h>

int main(){
	char* fifofile = "./mkfifofile";
	char* mknodfile = "./nodfifofile";
	int mkstatus, mknodstatus;
	mkstatus = mkfifo(fifofile, S_IRWXU);
	if(mkstatus == -1){
		perror("Error in creating file using mkfifo");
		return -1;
	}
	else{
		printf("File created successfully using mkfifo.\n");
	}
	mknodstatus = mknod(mknodfile, __S_IFIFO| S_IRWXU, 0);
	if(mknodstatus == -1){
		perror("Error in creating file using mknod");
                return -1;
        }
        else{
                printf("File created successfully mknod.\n");
        }
	return 0;
}


