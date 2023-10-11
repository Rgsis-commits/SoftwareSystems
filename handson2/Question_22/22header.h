#include <errno.h>
char *fifofile = "./myfifo";

void init()
{
    int status;

    status = mkfifo(fifofile, S_IRWXU);
    if (!(errno == EEXIST || status != -1))
        perror("Error while creating FIFO file fifoOne");
}