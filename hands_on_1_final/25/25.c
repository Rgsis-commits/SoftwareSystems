#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/wait.h>

int main(){
	int waitstatus;
	pid_t childA, childB, childC, terminated_child;
	if((childA = fork())==0){
		printf("First Child is created successfully.\n");
		printf("Child A is going to sleep for 5 second.\n");
		sleep(5);
		printf("Child A is waking up.\n");
		exit(0);
	}
	else{
		if((childB = fork()) == 0){
			printf("Second Child is created successfully.\n");
			printf("Child B is going to sleep for 10 second.\n");
			sleep(10);
			printf("Child B is waking up.\n");
			exit(0);
		}
		else{
			if((childC = fork()) == 0){
                        	printf("Third Child is created successfully.\n");
                        	printf("Child C is going to sleep for 15 second.\n");
                        	sleep(15);
                        	printf("Child C is waking up.\n");
                       		exit(0);
                	}
			else{
				terminated_child = waitpid(childB,&waitstatus,0);
				if(terminated_child == childB){
					if(waitstatus == 0){
						printf("Child B is exited with the exit status 0.\n");
					}
					else{
						printf("Child B is exited with the exit status %d.\n",waitstatus);
					}
				}
			}
		}
	}
	return 0;
}
