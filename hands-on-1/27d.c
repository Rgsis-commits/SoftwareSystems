#include<stdio.h>
#include<unistd.h>

int main(){
	char *args[] = {"ls", "-l", NULL};
        execv("/bin/ls", args);
        if("execlv")
                perror("Error in execlv");
        return 0;
}  
