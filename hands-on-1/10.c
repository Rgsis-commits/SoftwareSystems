#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int fd = open("testfile.txt", O_RDWR | O_CREAT | O_TRUNC, 0666);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }

    const char data1[] = "Hello12345"; // 10 bytes
    const char data2[] = "World67890"; // 10 bytes

    ssize_t bytes_written = write(fd, data1, sizeof(data1) - 1); // Write 10 bytes
    if (bytes_written == -1) {
        perror("Error writing data");
        close(fd);
        return 1;
    }

    off_t seek_result = lseek(fd, 10, SEEK_CUR); // Move the file pointer by 10 bytes
    if (seek_result == -1) {
        perror("Error seeking in the file");
        close(fd);
        return 1;
    }

    bytes_written = write(fd, data2, sizeof(data2) - 1); // Write 10 bytes
    if (bytes_written == -1) {
        perror("Error writing data");
        close(fd);
        return 1;
    }

    close(fd);

    return 0;
}

