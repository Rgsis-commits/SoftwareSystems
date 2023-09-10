/*
============================================================================
Name : 11b.c
Author : Rishav Chandel
Description : Write a program to open a file, duplicate the file descriptor and append the file with both the
descriptors and check whether the file is updated properly or not.
 b. use dup2
============================================================================
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

int main() {
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    // Duplicate the file descriptor using dup2
    int new_fd = dup2(fd, 4); // Duplicate fd to descriptor 3
    if (new_fd == -1) {
        perror("Error duplicating file descriptor");
        close(fd);
        return 1;
    }

    const char *data = "Appended data from descriptor 3.\n";
    ssize_t bytes_written = write(fd, data, strlen(data));
    if (bytes_written == -1) {
        perror("Error writing to file using fd");
        close(fd);
        return 1;
    }

    // Append using duplicated descriptor
    const char *data_dup = "Appended data from duplicated descriptor.\n";
    ssize_t bytes_written_dup = write(new_fd, data_dup, strlen(data_dup));
    if (bytes_written_dup == -1) {
        perror("Error writing to file using duplicated descriptor");
        close(fd);
        return 1;
    }

    close(fd);
    return 0;
}

