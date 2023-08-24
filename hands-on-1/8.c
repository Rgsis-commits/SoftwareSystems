#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("Wrong number of arguments given");
		return -1;
	}
	int fd_read = open(argv[1], O_RDONLY);
	char buffer[1024];
	ssize_t bytes_read;
	char line[1024];
	ssize_t linelength = 0;
	while(1){
		while((bytes_read = read(fd_read, buffer, sizeof(buffer)))>0){
			for(int i = 0; i<bytes_read; i++){
				if(buffer[i]!='\n'){
					line[linelength++] = buffer[i];
				}
				else{
					line[linelength] = '\0';
					printf("%s\n",line);
					linelength = 0;
				}
			}
		}
		if(bytes_read == -1){
			perror("Error in reading data");
			return -1;
		}
		if(bytes_read == 0)
			break;

	}
	int fd_close = close(fd_read);
	if(fd_close == -1){
		printf("Your are useless system cannot close a file descriptor.");
		return -1;	
	}
	return 0;
}
