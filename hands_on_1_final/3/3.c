/*
============================================================================
Name : 27.c
Author : Suraj Subedi
Description : Write a program to create a file and print the file descriptor value. Use creat ( ) system call
============================================================================
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> // Added for standard I/O functions

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("You didn't provide the correct number of arguments!");
        return 0;
    }

    int fd_read = open(argv[1], O_RDONLY | O_CREAT, 0644); // Added file mode (0644)
    if (fd_read == -1) {
        printf("Error opening or creating the file!");
        return 0;
    }
    
    printf("Your file has file descriptor %d\n", fd_read); // Added newline at the end
    
    int fd_close = close(fd_read);
    if (fd_close == -1) {
        printf("Error closing the file descriptor!");
        return 0;
    }

    return 0;
}
