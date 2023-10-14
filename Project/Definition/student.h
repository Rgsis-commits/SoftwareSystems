#ifndef STUDENT_CREDENTIALS
#define STUDENT_CREDENTIALS
#include "./courses.h"

struct student{
    char name[15];
    int login;
    char password[20];
    char active[1];
    struct courses courses_taken[200];
};

#endif