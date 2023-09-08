#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <stdlib.h>

int main(){
	int fd = open("hello.txt", O_RDWR|O_APPEND|O_CREAT, 0700);
	if(fd == -1){
		perror("Error in opening the file");
		return -1;
	}
	if(!fork()){
		printf("Child Turn");
		fflush(stdout);
                char *buff = NULL;
                size_t len = 0;
                ssize_t read;
                if((read = getline(&buff, &len, stdin)) != -1) {
                        write(fd,buff,read);
                }
                free(buff);

	}
	else{
		printf("Parent Turn: ");
		fflush(stdout);
		char *buff = NULL;
                size_t len = 0;
                ssize_t read;
                if((read = getline(&buff, &len, stdin)) != -1) {
                        write(fd,buff,read);
                }
		free(buff);

	}
	wait(0);
	close(fd);
	return 0;
}
