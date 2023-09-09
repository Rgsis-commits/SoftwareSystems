#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

int main(){
	int p_pid = getpid();
	int c_pid = fork();
	if(c_pid != -1){
		printf("Child is successfully created. Will run 2 times.\n");
	}
	if(c_pid!=0)
	{
		printf("Parent process id - %d\n",p_pid);
		printf("Child process id - %d\n",c_pid);
		printf("Parent is going to sleep.\n");
		fflush(stdin);
		sleep(120);
		printf("Parent is waking up\n");
	}
	else{
		printf("Child is exiting from parent\n");
		fflush(stdin);
		exit(0);
	}
	printf("Parent Process is terminating\n");
	return 0;
}
