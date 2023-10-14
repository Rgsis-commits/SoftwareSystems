#ifndef FACULTY_CREDENTIALS
#define FACULTY_CREDENTIALS
#include "./courses.h"

struct faculty{
    char name[50];
    int login;
    char password[50];
    struct courses courses_offered[200];
};

#endif