#include <stdio.h>
#include <unistd.h>
#include <sys/resource.h>
#include <sched.h>

int main(){
	int pid = getpid();
	int beforeset = getpriority(PRIO_PROCESS,pid);
	printf("The existing priority of process is %d.\n",beforeset);
	int setter;
	printf("Enter the priority you want the process to have:"); 
	scanf("%d",&setter);
	if(setter<-20 || setter>19){
		printf("The valid range is from -20 to 19.\n");
		return 0;
	}
	int set = setpriority(PRIO_PROCESS, pid, setter);
	if(set == -1){
		perror("Error in setting priority");
		return 0;
	}
	int afterset = getpriority(PRIO_PROCESS, pid);
	printf("The priority of process after setting is %d.\n",afterset);
	return 0;
}
