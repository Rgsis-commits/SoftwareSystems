#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sched.h>

int main(){
	pid_t pid;
	pid = getpid();
	int policy;
	policy = sched_getscheduler(pid);
	struct sched_param priority;
	priority.sched_priority = 10;
	int count = 3;
	while(count){
		switch(policy)
		{
			case SCHED_FIFO:
				printf("Current policy is FIFO\n");
	        		sched_setscheduler(pid, SCHED_RR, &priority);
        			policy = sched_getscheduler(pid);
       	 			printf("Current policy is %d\n", policy);
        			break;
	    		case SCHED_RR:
        			printf("Current policy is RR\n");
        			sched_setscheduler(pid, SCHED_FIFO, &priority);
        			policy = sched_getscheduler(pid);
	        		printf("Current policy is %d\n", policy);
        			break;
    			case SCHED_OTHER:
        			printf("Current policy is OTHER\n");
	        		sched_setscheduler(pid, SCHED_RR, &priority);
        			policy = sched_getscheduler(pid);
        			printf("Current policy is %d\n", policy);
        			break;
	    		default:
        			perror("Error while getting current policy\n");
		}
		count--;

	}	
	return 0;
	
}
