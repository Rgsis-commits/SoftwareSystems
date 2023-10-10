/*
============================================================================
Name : 19d.c
Author : Rishav Chandel
Description : Create a FIFO file by
              d. mknod system call
Date: 10th Oct, 2023.
============================================================================
*/

#include <sys/types.h> 
#include <sys/stat.h>  
#include <fcntl.h>     
#include <unistd.h>    
#include <stdio.h>     

void main()
{
    char *mkfifoName = "./mymkfifo";    
    char *mknodName = "./mymknod-fifo"; 
    int mknod_status; // 0 -> Success, -1 -> Error

    // Using `mknod` system call
    mknod_status = mknod(mknodName, __S_IFIFO | S_IRWXU, 0);

    if (mknod_status == -1)
        perror("Error while creating FIFO file!");
    else
        printf("Succesfully created FIFO file. Check using `ll` or `ls -l` command!\n");
}