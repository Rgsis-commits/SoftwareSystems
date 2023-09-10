/*
============================================================================
Name : 28.c
Author : Rishav Chandel
Description : Write a program to get maximum and minimum real time priority
============================================================================
*/
#include<sched.h>
#include<stdio.h>

int main(){
	int max_p = sched_get_priority_max(SCHED_RR);
	int min_p = sched_get_priority_min(SCHED_RR);
	printf("The range of the real time priority is between %d and %d,\n",min_p,max_p);
	return 0;
}
