#ifndef STUDENT_FUNCTIONS
#define STUDENT_FUNCTIONS

// Semaphores are necessary joint account due the design choice I've made
#include <sys/ipc.h>
#include <sys/sem.h>

struct student loggedInStudent;
int semIdentifier;

// Function Prototypes =================================

bool student_handler(int connFD);
bool get_course_details(int connFD);
bool enroll_course(int connFD);
//bool drop_course(int connFD);
bool change_password_student(int connFD);
//bool get_enrolled_courses(int connFD);

// =====================================================

// Function Definition =================================

bool student_handler(int connFD)
{
    if (login_handler(false, true, false, connFD, &loggedInStudent, NULL))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client

        // Get a semaphore for the user
        key_t semKey = ftok(STUDENT_FILE, loggedInStudent.login); // Generate a key based on the account number hence, different customers will have different semaphores

        union semun
        {
            int val; // Value of the semaphore
        } semSet;

        int semctlStatus;
        semIdentifier = semget(semKey, 1, 0); // Get the semaphore if it exists
        if (semIdentifier == -1)
        {
            semIdentifier = semget(semKey, 1, IPC_CREAT | 0700); // Create a new semaphore
            if (semIdentifier == -1)
            {
                perror("Error while creating semaphore!");
                _exit(1);
            }

            semSet.val = 1; // Set a binary semaphore
            semctlStatus = semctl(semIdentifier, 0, SETVAL, semSet);
            if (semctlStatus == -1)
            {
                perror("Error while initializing a binary sempahore!");
                _exit(1);
            }
        }

        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, STUDENT_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, STUDENT_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing FACULTY_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            bzero(readBuffer, sizeof(readBuffer));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for FACULTY_MENU");
                return false;
            }
            
            // printf("READ BUFFER : %s\n", readBuffer);
            int choice = atoi(readBuffer);
            // printf("CHOICE : %d\n", choice);
            switch (choice)
            {
            case 1:
                get_course_details(connFD);
                break;
            case 2:
                enroll_course(connFD);
                break;
            // case 3:
            //     drop_course(connFD);
            //     break;
            case 4:
                change_password_student(connFD);
                break;
            // case 5:
            //     get_enrolled_courses(connFD, loggedInStudent.userid);
            //     break;
            default:
                writeBytes = write(connFD, STUDENT_LOGOUT, strlen(STUDENT_LOGOUT));
                return false;
            }
        }
    }
    else
    {
        // CUSTOMER LOGIN FAILED
        return false;
    }
    return true;
}


bool get_course_details(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[10000];
    char tempBuffer[1000];
    struct courses coursesBuffer[1000];

    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));
    bzero(tempBuffer, sizeof(tempBuffer));

    struct faculty faculty;
    int customerFileDescriptor;

    customerFileDescriptor = open(COURSE_FILE, O_RDONLY);
    if (customerFileDescriptor == -1) {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Could not open course file in read only mode.");
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing FAILED_TO_OPEN_FILE message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }

    struct stat file_info;

    if (fstat(customerFileDescriptor, &file_info) == -1) {
        perror("Failed to get file info.");
        close(customerFileDescriptor);
        return -1;
    }

    int offset = lseek(customerFileDescriptor, 0, SEEK_SET);
    if (errno == EINVAL) {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Could not set seek pointer.");
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing POINTER_SEEK_PROBLEM message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1) {
        perror("Error while seeking to required faculty record!");
        return false;
    }

    struct flock lock = {F_RDLCK, SEEK_SET, offset, file_info.st_size, getpid()};
    lock.l_start = offset;

    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error while obtaining read lock on the Course file!");
        return false;
    }

    int i = 0;
    do {
        struct courses course;
        readBytes = read(customerFileDescriptor, &course, sizeof(struct courses));
        if (readBytes == -1) {
            perror("Error reading faculty record from file!");
            return false;
        }
        coursesBuffer[i] = course;
        i++;
    } while (readBytes > 0);

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcat(writeBuffer, "Courses available for you to pick - \n");

    for (int i = 0; i < 1000; i++) {
        if (coursesBuffer[i].cid >= 1 && coursesBuffer[i].cid == coursesBuffer[i - 1].cid) {
            break;
        }
        else if (coursesBuffer[i].cid == 0) {
            break;
        }
        else {
            if (coursesBuffer[i].offeredby != -1) {
                char temp[200];
                bzero(temp, sizeof(temp));
                sprintf(temp, "\tCourse %d : %s\n", coursesBuffer[i].cid, coursesBuffer[i].name);
                strcat(writeBuffer, temp);
            }
        }
    }

    strcat(writeBuffer, "\n\nYou'll now be redirected to the main menu...^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing faculty info to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return true;
}

bool change_password_student(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];
    char newPassword[1000];

    writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
    if (writeBytes == -1) {
        perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error reading old password response from client");
        return false;
    }

    if (strcmp(readBuffer, loggedInStudent.password) == 0) {
        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
        if (writeBytes == -1) {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading new password response from client");
            return false;
        }

        strcpy(newPassword, readBuffer);

        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
        if (writeBytes == -1) {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
            return false;
        }

        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1) {
            perror("Error reading new password reenter response from client");
            return false;
        }

        if (strcmp(readBuffer, newPassword) == 0) {
            strcpy(loggedInStudent.password, newPassword);

            int customerFileDescriptor = open(STUDENT_FILE, O_WRONLY);
            if (customerFileDescriptor == -1) {
                perror("Error opening customer file!");
                return false;
            }

            off_t offset = lseek(customerFileDescriptor, loggedInStudent.login * sizeof(struct faculty), SEEK_SET);
            if (offset == -1) {
                perror("Error seeking to the customer record!");
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct student), getpid()};
            int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1) {
                perror("Error obtaining write lock on customer record!");
                return false;
            }

            writeBytes = write(customerFileDescriptor, &loggedInStudent, sizeof(struct student));
            if (writeBytes == -1) {
                perror("Error storing updated customer password into customer record!");
                return false;
            }

            lock.l_type = F_UNLCK;
            lockingStatus = fcntl(customerFileDescriptor, F_SETLK, &lock);

            close(customerFileDescriptor);

            writeBytes = write(connFD, PASSWORD_CHANGE_SUCCESS, strlen(PASSWORD_CHANGE_SUCCESS));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

            return true;
        }
        else {
            writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_INVALID, strlen(PASSWORD_CHANGE_NEW_PASS_INVALID));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        }
    }
    else {
        writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS_INVALID, strlen(PASSWORD_CHANGE_OLD_PASS_INVALID));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    }

    return false;
}

bool enroll_course(int connFD) {
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[10000], tempBuffer[1000];
    struct student student = loggedInStudent;
    struct courses course;
    int customerFileDescriptor;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct student), getpid()};
    int courseId = -1;
    
    writeBytes = write(connFD, GET_COURSE_ID, strlen(GET_COURSE_ID));
    if (writeBytes == -1) {
        perror("Error while writing GET_COURSE_ID message to client!");
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1) {
        perror("Error getting course ID from client!");
        return false;
    }

    courseId = atoi(readBuffer);

    customerFileDescriptor = open(COURSE_FILE, O_RDWR);
    if (customerFileDescriptor == -1) {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, "Could not open course file in read only mode.");
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing FAILED_TO_OPEN_FILE message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    int offset = lseek(customerFileDescriptor, (courseId - 1) * sizeof(struct courses), SEEK_SET);
    if (errno == EINVAL) {
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, COURSE_ID_DOESNT_EXIT);
        strcat(writeBuffer, "^");
        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1) {
            perror("Error while writing COURSE_ID_DOESNT_EXIT message to client!");
            return false;
        }
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        return false;
    }
    else if (offset == -1) {
        perror("Error while seeking to required course record!");
        return false;
    }

    lock.l_start = offset;
    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error while obtaining read lock on the Course file!");
        return false;
    }

    readBytes = read(customerFileDescriptor, &course, sizeof(struct courses));
    if (readBytes == -1) {
        perror("Error reading student record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    for (int i = 0; i < 100; i++) {
        if (student.courses_taken[i].cid == 0) {
            student.courses_taken[i] = course;
            break;
        }
    }

    for (int i = 0; i < 100; i++) {
        if (course.enrolled_student[i] == 0) {
            course.enrolled_student[i] = student.login;
            break;
        }
    }

    customerFileDescriptor = open(STUDENT_FILE, O_WRONLY);
    if (customerFileDescriptor == -1) {
        perror("Error while opening customer file");
        return false;
    }

    offset = lseek(customerFileDescriptor, student.login * sizeof(struct student), SEEK_SET);
    if (offset == -1) {
        perror("Error while seeking to required student record!");
        return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error while obtaining write lock on student record!");
        return false;
    }

    writeBytes = write(customerFileDescriptor, &student, sizeof(struct student));
    if (writeBytes == -1) {
        perror("Error while writing update student info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);

    close(customerFileDescriptor);

    customerFileDescriptor = open(COURSE_FILE, O_WRONLY);
    if (customerFileDescriptor == -1) {
        perror("Error while opening course file");
        return false;
    }

    offset = lseek(customerFileDescriptor, (course.cid-1) * sizeof(struct courses), SEEK_SET);
    if (offset == -1) {
        perror("Error while seeking to required course record!");
        return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1) {
        perror("Error while obtaining write lock on course record!");
        return false;
    }

    writeBytes = write(customerFileDescriptor, &course, sizeof(struct courses));
    if (writeBytes == -1) {
        perror("Error while writing update student info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);

    close(customerFileDescriptor);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcat(writeBuffer, "You are now enrolled in the course.");

    strcat(writeBuffer, "\n\nYou'll now be redirected to the main menu...^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1) {
        perror("Error writing student info to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return true;
}

#endif