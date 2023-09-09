#include<stdio.h>
#include<unistd.h>

int main(){
	char *path = "/bin/ls";
	char *envp[] = {
		"USER=Rishav_Chandel",
		"HOME=/home/rishav-chandel",
		NULL
	};
        execle(path, "ls", "-l", NULL, envp);
        if("execle")
                perror("Error in execle");
        return 0;
}

