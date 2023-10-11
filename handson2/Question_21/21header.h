#include <errno.h>

char* file1 = "./myfifo1";
char* file2 = "./myfifo2";

void init()
{
    int status1, status2; 

    status1 = mkfifo(file1, S_IRWXU);
    if(!(errno == EEXIST || status1 != -1))
        perror("Error while defining first FIFO file!");
    
    status2 = mkfifo(file2, S_IRWXU);
    if(!(errno == EEXIST || status2 != -1))
        perror("Error while defining second FIFo file!");

}
