/*
============================================================================
Name : 19.c
Author : Rishav Chandel
Description :  Write a program to find out time taken to execute getpid system call. Use time stamp counter. 
============================================================================
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main() {
    struct timespec start_time, end_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);
    getpid();
    clock_gettime(CLOCK_MONOTONIC, &end_time);

    long long time = (end_time.tv_sec - start_time.tv_sec)*1000000000LL + (end_time.tv_nsec - start_time.tv_nsec);
    printf("The elapsed time for getting process ID is: %lldns\n",time);
    
    return 0;
}

