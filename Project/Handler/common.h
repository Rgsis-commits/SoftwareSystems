#ifndef COMMON_FUNCTIONS
#define COMMON_FUNCTIONS

#include <stdio.h>     // Import for `printf` & `perror`
#include <unistd.h>    // Import for `read`, `write & `lseek`
#include <string.h>    // Import for string functions
#include <stdbool.h>   // Import for `bool` data type
#include <sys/types.h> // Import for `open`, `lseek`
#include <sys/stat.h>  // Import for `open`
#include <fcntl.h>     // Import for `open`
#include <stdlib.h>    // Import for `atoi`
#include <errno.h>     // Import for `errno`

#include "../Definition/admin.h"
#include "../Definition/faculty.h"
#include "../Definition/student.h"
#include "../Definition/courses.h"
#include "../Definition/server-constants.h"

// Function Prototypes =================================

bool login_handler(bool isAdmin, bool isStudent, bool isFaculty, int connFD, struct student *ptrToStudentID, struct faculty *ptrToFacultyID);


// Function Definition =================================

bool login_handler(bool isAdmin, bool isStudent, bool isFaculty, int connFD, struct student *ptrToStudentID, struct faculty *ptrToFacultyID)
{
    ssize_t readBytes, writeBytes;            // Number of bytes written to / read from the socket
    char readBuffer[1000], writeBuffer[1000]; // Buffer for reading from / writing to the client
    char tempBuffer[1000];
    struct student stu;

    struct faculty fac;

    int ID;

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    // Get login message for respective user type
    if (isAdmin)
        strcpy(writeBuffer, ADMIN_LOGIN_WELCOME);
    else if(isFaculty)
        strcpy(writeBuffer, FACULTY_LOGIN_WELCOME);
    else
        strcpy(writeBuffer, STUDENT_LOGIN_WELCOME);
    // Append the request for LOGIN ID message
    strcat(writeBuffer, "\n");
    strcat(writeBuffer, LOGIN_ID);

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing WELCOME & LOGIN_ID message to the client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading login ID from client!");
        return false;
    }

    bool userFound = false;

    if (isAdmin)
    {
        if (strcmp(readBuffer, ADMIN_LOGIN_ID) == 0)
            userFound = true;
    }
    else if(isFaculty)
    {
        bzero(tempBuffer, sizeof(tempBuffer));
        strcpy(tempBuffer, readBuffer);
    
        ID = atoi(tempBuffer);
        int customerFileFD = open(FACULTY_FILE, O_RDONLY);
        if (customerFileFD == -1)
        {
            perror("Error opening faculty file in read mode!");
            return false;
        }

        off_t offset = lseek(customerFileFD, ID * sizeof(struct faculty), SEEK_SET);
        if (offset >= 0)
        {
            struct flock lock = {F_RDLCK, SEEK_SET, ID * sizeof(struct faculty), sizeof(struct faculty), getpid()};

            int lockingStatus = fcntl(customerFileFD, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining read lock on customer record!");
                return false;
            }

            readBytes = read(customerFileFD, &fac, sizeof(struct faculty));
            if (readBytes == -1)
            {
                ;
                perror("Error reading customer record from file!");
            }

            lock.l_type = F_UNLCK;
            fcntl(customerFileFD, F_SETLK, &lock);

            if(fac.login == ID)
                userFound = true;

            close(customerFileFD);
        }
        else
        {
            writeBytes = write(connFD, FACULTY_LOGIN_ID_DOESNT_EXIT, strlen(FACULTY_LOGIN_ID_DOESNT_EXIT));
        }
    }
    else{
        bzero(tempBuffer, sizeof(tempBuffer));
        strcpy(tempBuffer, readBuffer);
        ID = atoi(tempBuffer);
        int customerFileFD = open(STUDENT_FILE, O_RDONLY);
        if (customerFileFD == -1)
        {
            perror("Error opening faculty file in read mode!");
            return false;
        }

        off_t offset = lseek(customerFileFD, ID * sizeof(struct student), SEEK_SET);
        if (offset >= 0)
        {
            struct flock lock = {F_RDLCK, SEEK_SET, ID * sizeof(struct student), sizeof(struct student), getpid()};

            int lockingStatus = fcntl(customerFileFD, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining read lock on customer record!");
                return false;
            }

            readBytes = read(customerFileFD, &stu, sizeof(struct student));
            if (readBytes == -1)
            {
                ;
                perror("Error reading customer record from file!");
            }

            lock.l_type = F_UNLCK;
            fcntl(customerFileFD, F_SETLK, &lock);

            if(stu.login == ID)
                userFound = true;

            close(customerFileFD);
        }
        else
        {
            writeBytes = write(connFD, STUDENT_LOGIN_ID_DOESNT_EXIT, strlen(STUDENT_LOGIN_ID_DOESNT_EXIT));
        }
    }

    if (userFound)
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, PASSWORD, strlen(PASSWORD));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == 1)
        {
            perror("Error reading password from the client!");
            return false;
        }


        if (isAdmin)
        {
            if (strcmp(readBuffer, ADMIN_PASSWORD) == 0)
                return true;
        }
        else if(isFaculty)
        {
           
            if (strcmp(readBuffer, fac.password) == 0)
            {
                *ptrToFacultyID = fac;
                return true;
            }
        }
        else{
            
            if (strcmp(readBuffer, stu.password) == 0)
            {
                *ptrToStudentID = stu;
                return true;
            }
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_PASSWORD, strlen(INVALID_PASSWORD));
    }
    else
    {
        bzero(writeBuffer, sizeof(writeBuffer));
        writeBytes = write(connFD, INVALID_LOGIN, strlen(INVALID_LOGIN));
    }

    return false;
}

bool get_student_details(int connFD, int studentID)
{
    ssize_t readBytes, writeBytes;             // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[10000]; // A buffer for reading from / writing to the socket
    char tempBuffer[1000];

    struct student stu;
    int customerFileDescriptor;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct student), getpid()};

    if (studentID == -1)
    {
        writeBytes = write(connFD, GET_STUDENT_ID, strlen(GET_STUDENT_ID));
        if (writeBytes == -1)
        {
            perror("Error while writing GET_STUDENT_ID message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error getting student ID from client!");
            ;
            return false;
        }

        studentID = atoi(readBuffer);
    }

    customerFileDescriptor = open(STUDENT_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
        // Student File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, STUDENT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing STUDENT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    struct stat file_info;

    if(fstat(customerFileDescriptor, &file_info) == -1){
        perror("Failed to get file info.");
        close(customerFileDescriptor);
        return -1;
    }


    if(studentID * sizeof(struct student) >= file_info.st_size) {
        // Student record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, STUDENT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing STUDENT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    int offset = lseek(customerFileDescriptor, studentID * sizeof(struct student), SEEK_SET);
    if (errno == EINVAL)
    {
        // Student record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, STUDENT_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing STUDENT_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required student record!");
        return false;
    }
    lock.l_start = offset;

    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining read lock on the Student file!");
        return false;
    }

    readBytes = read(customerFileDescriptor, &stu, sizeof(struct student));
    if (readBytes == -1)
    {
        perror("Error reading student record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Student Details - \n\tID : %d\n\tName : %s\n\tActivity: %s", stu.login, stu.name, stu.active);

    strcat(writeBuffer, "\n\nYou'll now be redirected to the main menu...^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing student info to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return true;
}

bool get_faculty_details(int connFD, int facultyID)
{
    ssize_t readBytes, writeBytes;             // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[10000]; // A buffer for reading from / writing to the socket
    char tempBuffer[1000];

    struct faculty fac;
    int customerFileDescriptor;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct faculty), getpid()};

    if (facultyID == -1)
    {
        writeBytes = write(connFD, GET_FACULTY_ID, strlen(GET_FACULTY_ID));
        if (writeBytes == -1)
        {
            perror("Error while writing GET_FACULTY_ID message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error getting faculty ID from client!");
            ;
            return false;
        }

        facultyID = atoi(readBuffer);
    }

    customerFileDescriptor = open(FACULTY_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
        // Student File doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, FACULTY_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing FACULTY_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    struct stat file_info;

    if(fstat(customerFileDescriptor, &file_info) == -1){
        perror("Failed to get file info.");
        close(customerFileDescriptor);
        return -1;
    }


    if(facultyID * sizeof(struct faculty) >= file_info.st_size) {
        // Faculty record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, FACULTY_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing FACULTY_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    int offset = lseek(customerFileDescriptor, facultyID * sizeof(struct faculty), SEEK_SET);
    if (errno == EINVAL)
    {
        // Faculty record doesn't exist
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, FACULTY_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error while writing FACULTY_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1)
    {
        perror("Error while seeking to required faculty record!");
        return false;
    }
    lock.l_start = offset;

    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
        perror("Error while obtaining read lock on the faculty file!");
        return false;
    }

    readBytes = read(customerFileDescriptor, &fac, sizeof(struct faculty));
    if (readBytes == -1)
    {
        perror("Error reading faculty record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "Faculty Details - \n\tID : %d\n\tName : %s", fac.login, fac.name);

    strcat(writeBuffer, "\n\nYou'll now be redirected to the main menu...^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error writing faculty info to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return true;
}





#endif