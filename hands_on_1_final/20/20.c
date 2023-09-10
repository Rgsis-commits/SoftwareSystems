/*
============================================================================
Name : 20.c
Author : Rishav Chandel
Description : Find out the priority of your running program. Modify the priority with nice command.
============================================================================
*/
#include <unistd.h> // Import for `nice` system call
#include <stdio.h>  // Import for `printf` function
#include <stdlib.h> // `atoi` conversion from string to int

void main()
{
    int priority, newp;
    priority = nice(0); // Get the priorty by adding 0 to current priorty
    printf("Current priority: %d\n", priority);
    printf("Enter the new value which you want to add to current priority: ");
    scanf("%d",&newp);
    priority = nice(newp); // Adds `newp` to the current priority
    printf("New priority: %d\n", priority);
}
