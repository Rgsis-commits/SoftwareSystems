#include <sys/types.h> // Imports for using `stat` system call
#include <sys/stat.h>  // Imports for using `stat` system call
#include <unistd.h>    // Imports for using `stat` system call
#include <fcntl.h>     // Import for `O_RDONLY` flag
#include <stdio.h>     // Import for `printf`

void main(int argc, char *argv[])
{
    struct stat statbuf;
    int fileDescriptor;

    if (argc != 2)
        printf("Pass the name of the file as the argument");
    else
    {
        fileDescriptor = open(argv[1], O_RDONLY);

        fstat(fileDescriptor, &statbuf);

        if (S_ISREG(statbuf.st_mode))
            write(STDOUT_FILENO, "Regular File\n", 13);

        else if (S_ISDIR(statbuf.st_mode))
            write(STDOUT_FILENO, "Directory File\n", 15);

        else if (S_ISCHR(statbuf.st_mode))
            write(STDOUT_FILENO, "Character File\n", 15);

        else if (S_ISBLK(statbuf.st_mode))
            write(STDOUT_FILENO, "Block File\n", 11);

        else if (S_ISFIFO(statbuf.st_mode))
            write(STDOUT_FILENO, "FIFO File\n", 10);

        else if (S_ISLNK(statbuf.st_mode))
            write(STDOUT_FILENO, "Symbolic Link File\n", 19);

        else if (S_ISSOCK(statbuf.st_mode))
            write(STDOUT_FILENO, "Socket\n", 7);

        else
            write(STDOUT_FILENO, "Error\n", 6);
    }
}
