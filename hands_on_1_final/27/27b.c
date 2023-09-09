#include<stdio.h>
#include<unistd.h>

int main(){
        execlp("ls", "ls", "-l", NULL);
        if("execl")
                perror("Error in execl");
        return 0;
}

