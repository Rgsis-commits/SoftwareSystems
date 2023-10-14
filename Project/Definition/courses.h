#ifndef COURSE_RECORDS
#define COURSE_RECORDS

struct courses{
    int cid;
    char name[50];
    int offeredby;
    int count;
    int limit;
    int enrolled_student[200];
};

#endif