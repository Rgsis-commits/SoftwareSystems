#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Wrong number of arguments");
		return -1;
	}
	int fd_append = open(argv[1],O_WRONLY| O_APPEND);
	if(fd_append == -1){
		printf("Your system cannot open a file.");
		return -1;
	}
	int dup_fd_append = dup(fd_append);
	char* dataappend = "NewOrder";
	int bytes_written = write(fd_append,dataappend,strlen(dataappend));
	bytes_written = write(dup_fd_append,dataappend,strlen(dataappend));
	if(bytes_written == -1){
		perror("Error in appending data");
		return -1;
	}
	int fd_close = close(fd_append);
	if(fd_close == -1){
		printf("Your system cannot close file descriptor");
		return -1;
	}
	return 0;
}
