#include<stdio.h>
#include<unistd.h>
#include<sys/wait.h>
#include<stdlib.h>

int main(){
	int p_pid = getpid();
	int c_pid = fork();
	if(c_pid == 0){
                printf("Child is sleeping.\n");
                sleep(20);
                printf("Child is waking up\n");
		exit(0);
	}
	else{
		printf("Parent process id is %d\n",p_pid);
		printf("Child process id is %d\n",c_pid);
		printf("Parent is finishing execution.");
		exit(0);
	}
	return 0;
}
