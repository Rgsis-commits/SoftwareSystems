#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    int flags = fcntl(fd, F_GETFL);
    if (flags == -1) {
        perror("Error getting file status flags");
        close(fd);
        return 1;
    }

    int access_mode = flags & O_ACCMODE;
    if (access_mode == O_RDONLY) {
        printf("File opened in read-only mode\n");
    } else if (access_mode == O_WRONLY) {
        printf("File opened in write-only mode\n");
    } else if (access_mode == O_RDWR) {
        printf("File opened in read-write mode\n");
    } else {
        printf("File opened with unknown access mode\n");
    }

    int fd_close = close(fd);
    return 0;
}

