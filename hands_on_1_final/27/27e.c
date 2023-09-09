#include<stdio.h>
#include<unistd.h>

int main(){
        char *args[] = {"ls", "-l", NULL};
        execv("/bin/ls", args);
        perror("Error in execvp");
        return 0;
}
