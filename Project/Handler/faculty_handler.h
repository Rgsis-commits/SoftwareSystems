#ifndef FACULTY_FUNCTIONS
#define FACULTY_FUNCTIONS

#include <sys/ipc.h>
#include <sys/sem.h>

#include "../Definition/courses.h"

struct faculty loggedInFaculty;
int semIdentifier;

// Function Prototypes =================================

bool faculty_handler(int connFD);
bool get_offering_courses(int connFD);
int add_course(int connFD);
bool remove_course(int connFD);
bool update_course(int connFD);
bool change_password(int connFD);


// =====================================================

// Function Definition =================================

bool faculty_handler(int connFD)
{
    if (login_handler(false, false, true, connFD, NULL, &loggedInFaculty))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client

        // Get a semaphore for the user
        key_t semKey = ftok(FACULTY_FILE, loggedInFaculty.login); // Generate a key based on the account number hence, different customers will have different semaphores

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
        strcpy(writeBuffer, FACULTY_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, FACULTY_MENU);
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
                get_offering_courses(connFD);
                break;
            case 2:
                add_course(connFD);
                break;
            // case 3:
            //     remove_course(connFD);
            //     break;
            case 4:
                update_course(connFD);
                break;
            case 5:
                change_password(connFD);
                break;
            default:
                writeBytes = write(connFD, FACULTY_LOGOUT, strlen(FACULTY_LOGOUT));
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

// ADD Course
int add_course(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];
    bzero(readBuffer, sizeof(readBuffer));
    bzero(writeBuffer, sizeof(writeBuffer));

    struct courses newCourse, previousCourse;
    // struct stat file_info;

    int customerFileDescriptor = open(COURSE_FILE, O_RDONLY);
    if (customerFileDescriptor == -1 && errno == ENOENT)
    {
        // Course file was never created
        newCourse.cid = 1;
    }
    else if (customerFileDescriptor == -1)
    {
        perror("Error while opening course file");
        return -1;
    }
    else
    {
        int offset = lseek(customerFileDescriptor, -sizeof(struct courses), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last Course record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct courses), getpid()};
        int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Course record!");
            return false;
        }

        readBytes = read(customerFileDescriptor, &previousCourse, sizeof(struct courses));
        if (readBytes == -1)
        {
            perror("Error while reading Course record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);

        close(customerFileDescriptor);

        newCourse.cid = previousCourse.cid + 1;
    }

    // Enter Course name
    writeBytes = write(connFD, FACULTY_ADD_COURSE_NAME, sizeof(FACULTY_ADD_COURSE_NAME));
    if (writeBytes == -1)
    {
        perror("Error writing FACULTY_ADD_COURSE_NAME message to client!");
        return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading course name response from client!");
        ;
        return false;
    }

    strcpy(newCourse.name, readBuffer);

    // Enter Course offered by
    newCourse.offeredby = loggedInFaculty.login;

    // Enter Course registered count
    newCourse.count = 0;

    // Enter Course registered limit
    newCourse.limit = 150;
    bzero(newCourse.enrolled_student, sizeof(newCourse.enrolled_student));
    
    for(int i = 0; i<100; i++){
        if(loggedInFaculty.courses_offered[i].cid == 0){
            loggedInFaculty.courses_offered[i] = newCourse;
            break;
        }
    }

    

    customerFileDescriptor = open(COURSE_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (customerFileDescriptor == -1)
    {
        perror("Error while creating / opening course file!");
        return false;
    }
    writeBytes = write(customerFileDescriptor, &newCourse, sizeof(newCourse));
    if (writeBytes == -1)
    {
        perror("Error while writing course record to file!");
        return false;
    }
    close(customerFileDescriptor);
    customerFileDescriptor = open(FACULTY_FILE, O_WRONLY);
    if (customerFileDescriptor == -1)
    {
    perror("Error while opening faculty file");
    return false;
    }
    int offset = lseek(customerFileDescriptor, loggedInFaculty.login * sizeof(struct faculty), SEEK_SET);
    if (offset == -1)
    {
    perror("Error while seeking to required faculty record!");
    return false;
    }
    struct flock lock;
    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
    perror("Error while obtaining write lock on faculty record!");
    return false;
    }

    writeBytes = write(customerFileDescriptor, &loggedInFaculty, sizeof(struct faculty));
    if (writeBytes == -1)
    {
    perror("Error while writing update faculty info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);

    close(customerFileDescriptor);

    //close(customerFileDescriptor);


    bzero(writeBuffer, sizeof(writeBuffer));
    strcat(writeBuffer, "New course has been added.^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error sending customer loginID and password to the client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return newCourse.cid;
}


// Courses offered
bool get_offering_courses(int connFD) {
    ssize_t readBytes, writeBytes;             // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[10000]; // A buffer for reading from / writing to the socket
    char tempBuffer[1000];

    struct faculty faculty;
    int customerFileDescriptor;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct faculty), getpid()};

    int facultyId = loggedInFaculty.login;

    customerFileDescriptor = open(FACULTY_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
        // Faculty File doesn't exist
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

    // bzero(testbuffer, sizeof(testbuffer));
    // sprintf(testbuffer, "Eta ami %ld, size %ld",studentId * sizeof(struct Student), file_info.st_size);
    // writeBytes = write(connFD, testbuffer, strlen(testbuffer));

    if(facultyId * sizeof(struct faculty) >= file_info.st_size) {
        // Student record doesn't exist
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

    int offset = lseek(customerFileDescriptor, facultyId * sizeof(struct faculty), SEEK_SET);
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
        perror("Error while obtaining read lock on the Faculty file!");
        return false;
    }

    readBytes = read(customerFileDescriptor, &faculty, sizeof(struct faculty));
    if (readBytes == -1)
    {
        perror("Error reading faculty record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcat(writeBuffer, "Courses offered by you - \n");

    for(int i=0; i<100; i++) {
        if(loggedInFaculty.courses_offered[i].cid == 0){
            break;
        }
        else{
            char temp[200];
            bzero(temp, sizeof(temp));
            sprintf(temp, "\tCourse %d : %s", i+1, loggedInFaculty.courses_offered[i].name);
            strcat(writeBuffer, temp);
        }
    }

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

// Remove courses




// change password
bool change_password(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    char newPassword[1000];

    
    writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS, strlen(PASSWORD_CHANGE_OLD_PASS));
    if (writeBytes == -1)
    {
        perror("Error writing PASSWORD_CHANGE_OLD_PASS message to client!");
        
        return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading old password response from client");
        
        return false;
    }

    if (strcmp(readBuffer ,loggedInFaculty.password) == 0)
    {
        // Password matches with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS, strlen(PASSWORD_CHANGE_NEW_PASS));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS message to client!");
            
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password response from client");
            
            return false;
        }

        strcpy(newPassword, readBuffer);

        writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_RE, strlen(PASSWORD_CHANGE_NEW_PASS_RE));
        if (writeBytes == -1)
        {
            perror("Error writing PASSWORD_CHANGE_NEW_PASS_RE message to client!");
            
            return false;
        }
        bzero(readBuffer, sizeof(readBuffer));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer));
        if (readBytes == -1)
        {
            perror("Error reading new password reenter response from client");
            
            return false;
        }

        if (strcmp(readBuffer, newPassword) == 0)
        {
            // New & reentered passwords match

            strcpy(loggedInFaculty.password, newPassword);

            int customerFileDescriptor = open(FACULTY_FILE, O_WRONLY);
            if (customerFileDescriptor == -1)
            {
                perror("Error opening customer file!");
                
                return false;
            }

            off_t offset = lseek(customerFileDescriptor, loggedInFaculty.login * sizeof(struct faculty), SEEK_SET);
            if (offset == -1)
            {
                perror("Error seeking to the customer record!");
                
                return false;
            }

            struct flock lock = {F_WRLCK, SEEK_SET, offset, sizeof(struct faculty), getpid()};
            int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
            if (lockingStatus == -1)
            {
                perror("Error obtaining write lock on customer record!");
                
                return false;
            }

            writeBytes = write(customerFileDescriptor, &loggedInFaculty, sizeof(struct faculty));
            if (writeBytes == -1)
            {
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
        else
        {
            // New & reentered passwords don't match
            writeBytes = write(connFD, PASSWORD_CHANGE_NEW_PASS_INVALID, strlen(PASSWORD_CHANGE_NEW_PASS_INVALID));
            readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
        }
    }
    else
    {
        // Password doesn't match with old password
        writeBytes = write(connFD, PASSWORD_CHANGE_OLD_PASS_INVALID, strlen(PASSWORD_CHANGE_OLD_PASS_INVALID));
        readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    }

    

    return false;
}


bool update_course(int connFD){
    ssize_t readBytes, writeBytes;             // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[10000]; // A buffer for reading from / writing to the socket
    char tempBuffer[1000];

    int customerFileDescriptor;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct faculty), getpid()};

    int facultyId = loggedInFaculty.login;

    customerFileDescriptor = open(FACULTY_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
        // Faculty File doesn't exist
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


    if(facultyId * sizeof(struct faculty) >= file_info.st_size) {
        // Student record doesn't exist
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

    int offset = lseek(customerFileDescriptor, facultyId * sizeof(struct faculty), SEEK_SET);
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
        perror("Error while obtaining read lock on the Faculty file!");
        return false;
    }

    readBytes = read(customerFileDescriptor, &loggedInFaculty, sizeof(struct faculty));
    if (readBytes == -1)
    {
        perror("Error reading faculty record from file!");
        return false;
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    bzero(writeBuffer, sizeof(writeBuffer));
    strcat(writeBuffer, "Courses offered by you - \n");

    for(int i=0; i<100; i++) {
        if(loggedInFaculty.courses_offered[i].cid == 0){
            break;
        }
        else{
            char temp[200];
            bzero(temp, sizeof(temp));
            sprintf(temp, "\tCourse %d : %s", i+1, loggedInFaculty.courses_offered[i].name);
            strcat(writeBuffer, temp);
        }
    }

    strcat(writeBuffer, "^");

    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    
    if (writeBytes == -1)
    {
        perror("Error writing courses info to client!");
        return false;
    }
    bzero(writeBuffer, sizeof(writeBuffer));
    writeBytes = write(connFD, "\nEnter the ID of the course you want to update: ^",sizeof("\nEnter the ID of the course you want to update: ^"));

    if (writeBytes == -1)
    {
        perror("Error writing message info to client!");
        return false;
    }
    bzero(readBuffer,sizeof(readBuffer));
    readBytes = read(connFD,readBuffer,sizeof(readBytes));
    if (readBytes == -1)
    {
        perror("Error reading message from to client!");
        return false;
    }
    int temp = atoi(readBuffer);
    struct courses prevc;
    int coursesFileDescriptor = open(COURSE_FILE, O_RDONLY);
    if (customerFileDescriptor == -1)
    {
    // Course File doesn't exist
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, COURSE_ID_DOESNT_EXIT);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
    perror("Error while writing COURSE_ID_DOESNT_EXIT message to client!");
    return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return false;
    }
    
    offset = lseek(coursesFileDescriptor, (temp-1) * sizeof(struct courses), SEEK_SET);
    if (errno == EINVAL)
    {
    // Courses record doesn't exist
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, COURSES_ID_DOESNT_EXIT);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
    perror("Error while writing COURSES_ID_DOESNT_EXIT message to client!");
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

    struct flock lockl = {F_RDLCK, SEEK_SET, offset, sizeof(struct courses), getpid()};

    // lockl the record to be read
    lockingStatus = fcntl(coursesFileDescriptor, F_SETLKW, &lockl);
    if (lockingStatus == -1)
    {
    perror("Couldn't obtain lock on courses record!");
    return false;
    }

    readBytes = read(coursesFileDescriptor, &prevc, sizeof(struct courses));
    if (readBytes == -1)
    {
    perror("Error while reading faculty record from the file!");
    return false;
    }

    // Unlock the record
    lockl.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lockl);

    close(coursesFileDescriptor);

    writeBytes = write(connFD, COURSE_MOD_FACULTY_MENU, strlen(COURSE_MOD_FACULTY_MENU));
    if (writeBytes == -1)
    {
    perror("Error while writing COURSE_MOD_FACULTY_MENU message to client!");
    return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
    perror("Error while getting faculty modification menu choice from client!");
    return false;
    }



}
           




#endif