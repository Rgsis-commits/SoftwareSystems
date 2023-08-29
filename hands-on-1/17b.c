#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(){
	int fd = open("Ticket.txt",O_RDWR);
	if(fd == -1){
		printf("Error in openning the file");
		return -1;
	}
	struct flock lock;
	lock.l_type = F_WRLCK;	
	lock.l_whence = SEEK_SET;
	lock.l_start = 0;
	lock.l_len = 0;
	lock.l_pid = getpid();
	printf("Entering into ticket reservation section...\n");
	if(fcntl(fd, F_SETLKW, &lock)==-1){
		perror("Error in setting lock");
		close(fd);
		return -1;
	}
	int buf;
       	read(fd,&buf,sizeof(buf));
	buf++;
	printf("The ticket no %d is reserved for you.\n",buf);
	lseek(fd,0,SEEK_SET);
	write(fd,&buf,sizeof(buf));
	printf("Press enter to confirm");
	getchar();
	lock.l_type = F_UNLCK;
	if(fcntl(fd, F_SETLK, &lock) == -1){
		perror("Error in releasing the lock");
		close(fd);
		return -1;
	}
	close(fd);
	return 0;

}
