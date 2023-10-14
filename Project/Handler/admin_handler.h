#ifndef ADMIN_FUNCTIONS
#define ADMIN_FUNCTIONS

#include "./common.h"

// Function Prototypes =================================

bool admin_handler(int connFD);
int add_student(int connFD);
int add_faculty(int connFD);
bool activate_student(int connFD, char act);
bool modify_student_details(int connFD);
bool modify_faculty_details(int connFD);

// =====================================================

// Function Definition =================================

// =====================================================

bool admin_handler(int connFD)
{

    if (login_handler(true, false, false, connFD, NULL, NULL))
    {
        ssize_t writeBytes, readBytes;            // Number of bytes read from / written to the client
        char readBuffer[1000], writeBuffer[1000]; // A buffer used for reading & writing to the client
        bzero(writeBuffer, sizeof(writeBuffer));
        strcpy(writeBuffer, ADMIN_LOGIN_SUCCESS);
        while (1)
        {
            strcat(writeBuffer, "\n");
            strcat(writeBuffer, ADMIN_MENU);
            writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
            if (writeBytes == -1)
            {
                perror("Error while writing ADMIN_MENU to client!");
                return false;
            }
            bzero(writeBuffer, sizeof(writeBuffer));

            readBytes = read(connFD, readBuffer, sizeof(readBuffer));
            if (readBytes == -1)
            {
                perror("Error while reading client's choice for ADMIN_MENU");
                return false;
            }

            int choice = atoi(readBuffer);
            switch (choice)
            {
            case 1:
                add_student(connFD);
                break;
            case 2:
                get_student_details(connFD, -1);
                break;
            case 3: 
                add_faculty(connFD);
                break;
            case 4:
                get_faculty_details(connFD, -1);
                break;
            case 5:
                activate_student(connFD,'1');
                break;
            case 6:
                activate_student(connFD,'0');
                break;
            case 7:
                modify_student_details(connFD);
                break;
            case 8:
                modify_faculty_details(connFD);
                break;
            default:
                writeBytes = write(connFD, ADMIN_LOGOUT, strlen(ADMIN_LOGOUT));
                return false;
            }
        }
    }
    else
    {
        // ADMIN LOGIN FAILED
        return false;
    }
    return true;
}

int add_student(int connFD)
{
    int readBytes = 0, writeBytes = 0;
    char readBuffer[1000], writeBuffer[1000];
    bzero(readBuffer, sizeof(readBuffer));
    bzero(readBuffer, sizeof(readBuffer));
    struct student newStudent, prevStudent;

    int customerFileDescriptor = open(STUDENT_FILE, O_RDONLY);
    if (customerFileDescriptor == -1 && errno == ENOENT)
    {
        // Student file was never created
        newStudent.login = 0;
    }
    else if (customerFileDescriptor == -1)
    {
        perror("Error while opening customer file");
        return -1;
    }
    else
    {
        int offset = lseek(customerFileDescriptor, -sizeof(struct student), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last student record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct student), getpid()};
        int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on Customer record!");
            return false;
        }

        readBytes = read(customerFileDescriptor, &prevStudent, sizeof(struct student));
        if (readBytes == -1)
        {
            perror("Error while reading Customer record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);

        close(customerFileDescriptor);

        newStudent.login = prevStudent.login + 1;
    }
    writeBytes = write(connFD, ADMIN_ADD_STUDENT_NAME, sizeof(ADMIN_ADD_STUDENT_NAME));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_STUDENT_NAME message to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading student name response from client!");
        ;
        return false;
    }

    strcpy(newStudent.name, readBuffer);
    
    //Student active
    strcpy(newStudent.active, "1");
    // Password setup
    char hashedPassword[1000];
    strcpy(hashedPassword, AUTOGEN_PASSWORD);
    strcpy(newStudent.password, hashedPassword);


    customerFileDescriptor = open(STUDENT_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (customerFileDescriptor == -1)
    {
        perror("Error while creating / opening student file!");
        return false;
    }
   
    writeBytes = write(customerFileDescriptor, &newStudent, sizeof(newStudent));
    if (writeBytes == -1)
    {
        perror("Error while writing Student record to file!");
        return false;
    }

    close(customerFileDescriptor);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%s-%d\n%s%s", ADMIN_ADD_STUDENT_AUTOGEN_LOGIN, newStudent.name, newStudent.login, ADMIN_ADD_STUDENT_AUTOGEN_PASSWORD, AUTOGEN_PASSWORD);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error sending student loginID and password to the client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return newStudent.login;
}





int add_faculty(int connFD)
{
    int readBytes = 0, writeBytes = 0;
    char readBuffer[1000], writeBuffer[1000];
    bzero(readBuffer, sizeof(readBuffer));
    bzero(readBuffer, sizeof(readBuffer));

    struct faculty newFaculty, prevFaculty;

    int customerFileDescriptor = open(FACULTY_FILE, O_RDONLY);
    if (customerFileDescriptor == -1 && errno == ENOENT)
    {
        // Customer file was never created
        newFaculty.login = 0;
    }
    else if (customerFileDescriptor == -1)
    {
        perror("Error while opening faculty file");
        return -1;
    }
    else
    {
        int offset = lseek(customerFileDescriptor, -sizeof(struct student), SEEK_END);
        if (offset == -1)
        {
            perror("Error seeking to last faculty record!");
            return false;
        }

        struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct faculty), getpid()};
        int lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
        if (lockingStatus == -1)
        {
            perror("Error obtaining read lock on faculty record!");
            return false;
        }

        readBytes = read(customerFileDescriptor, &prevFaculty, sizeof(struct faculty));
        if (readBytes == -1)
        {
            perror("Error while reading faculty record from file!");
            return false;
        }

        lock.l_type = F_UNLCK;
        fcntl(customerFileDescriptor, F_SETLK, &lock);

        close(customerFileDescriptor);

        newFaculty.login = prevFaculty.login + 1;
    }

    writeBytes = write(connFD, ADMIN_ADD_FACULTY_NAME, sizeof(ADMIN_ADD_FACULTY_NAME));
    if (writeBytes == -1)
    {
        perror("Error writing ADMIN_ADD_FACULTY_NAME message to client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
        perror("Error reading faculty name response from client!");
        ;
        return false;
    }

    strcpy(newFaculty.name, readBuffer);
   
    // Password setup
    char hashedPassword[1000];
    strcpy(hashedPassword, AUTOGEN_PASSWORD);
    strcpy(newFaculty.password, hashedPassword);

    //Faculty courses
    bzero(newFaculty.courses_offered, sizeof(newFaculty.courses_offered));
    customerFileDescriptor = open(FACULTY_FILE, O_CREAT | O_APPEND | O_WRONLY, S_IRWXU);
    if (customerFileDescriptor == -1)
    {
        perror("Error while creating / opening faculty file!");
        return false;
    }
    writeBytes = write(customerFileDescriptor, &newFaculty, sizeof(newFaculty));
    if (writeBytes == -1)
    {
        perror("Error while writing faculty record to file!");
        return false;
    }

    close(customerFileDescriptor);

    bzero(writeBuffer, sizeof(writeBuffer));
    sprintf(writeBuffer, "%s%s-%d\n%s%s", ADMIN_ADD_FACULTY_AUTOGEN_LOGIN, newFaculty.name, newFaculty.login, ADMIN_ADD_FACULTY_AUTOGEN_PASSWORD, AUTOGEN_PASSWORD);
    strcat(writeBuffer, "^");
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
        perror("Error sending faculty loginID and password to the client!");
        return false;
    }

    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return newFaculty.login;
}

bool activate_student(int connFD, char act){
    ssize_t readBytes, writeBytes;             // Number of bytes read from / written to the socket
    char readBuffer[1000], writeBuffer[10000]; // A buffer for reading from / writing to the socket
    char tempBuffer[1000];
    bzero(readBuffer, sizeof(readBuffer));
    bzero(readBuffer, sizeof(readBuffer));

    struct student stu;
    int customerFileDescriptor;
    struct flock lock = {F_RDLCK, SEEK_SET, 0, sizeof(struct student), getpid()};
    int studentID = -1;
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

    customerFileDescriptor = open(STUDENT_FILE, O_RDWR);
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

    if(stu.active[0] == act) {
        bzero(writeBuffer, sizeof(writeBuffer));
        if(act == '0')
            strcat(writeBuffer, "Student is already deactivated.\n");
        else
            strcat(writeBuffer, "Student is already activated.\n");

        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing student info to client!");
            return false;
        }
    }
    else{
        offset = lseek(customerFileDescriptor, -sizeof(struct student), SEEK_CUR);
        if (offset == -1)
        {
            perror("Error seeking to last Customer record!");
            return false;
        }
        stu.active[0] = act;
        writeBytes = write(customerFileDescriptor, &stu, sizeof(stu));
        if (writeBytes == -1)
        {
            perror("Error while writing Faculty record to file!");
            return false;
        }
        if(act == '0')
            strcat(writeBuffer, "Student is now deactivated.\n");
        else
            strcat(writeBuffer, "Student is now activated.\n");

        writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
        if (writeBytes == -1)
        {
            perror("Error writing student info to client!");
            return false;
        }
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    //bzero(writeBuffer, sizeof(writeBuffer));
    
    // sprintf(writeBuffer, "Student Details - \n\tID : %d\n\tName : %s\n\tCourses: %s\n\tActivity: %s", stu.login, stu.name, stu.courses, stu.active);

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

// Modify Student info
bool modify_student_details(int connFD)
{
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct student stu;

    int studentID;

    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, ADMIN_MOD_STUDENT_ID, strlen(ADMIN_MOD_STUDENT_ID));
    if (writeBytes == -1)
    {
    perror("Error while writing ADMIN_MOD_STUDENT_ID message to client!");
    return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
    perror("Error while reading student ID from client!");
    return false;
    }

    studentID = atoi(readBuffer);

    int customerFileDescriptor = open(STUDENT_FILE, O_RDONLY);
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
    
    offset = lseek(customerFileDescriptor, studentID * sizeof(struct student), SEEK_SET);
    if (errno == EINVAL)
    {
    // Customer record doesn't exist
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

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct student), getpid()};

    // Lock the record to be read
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
    perror("Couldn't obtain lock on student record!");
    return false;
    }

    readBytes = read(customerFileDescriptor, &stu, sizeof(struct student));
    if (readBytes == -1)
    {
    perror("Error while reading student record from the file!");
    return false;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    close(customerFileDescriptor);

    writeBytes = write(connFD, ADMIN_MOD_STUDENT_MENU, strlen(ADMIN_MOD_STUDENT_MENU));
    if (writeBytes == -1)
    {
    perror("Error while writing ADMIN_MOD_STUDENT_MENU message to client!");
    return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
    perror("Error while getting student modification menu choice from client!");
    return false;
    }

    int choice = atoi(readBuffer);
    if (choice == 0)
    { // A non-numeric string was passed to atoi
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
    perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
    return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    switch (choice)
    {
    case 1:
    writeBytes = write(connFD, ADMIN_MOD_NEW_NAME, strlen(ADMIN_MOD_NEW_NAME));
    if (writeBytes == -1)
    {
    perror("Error while writing ADMIN_MOD_NEW_NAME message to client!");
    return false;
    }
    readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
    perror("Error while getting response for student's new name from client!");
    return false;
    }
    strcpy(stu.name, readBuffer);
    break;

    default:
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, INVALID_MENU_CHOICE);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
    perror("Error while writing INVALID_MENU_CHOICE message to client!");
    return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return false;
    }

    customerFileDescriptor = open(STUDENT_FILE, O_WRONLY);
    if (customerFileDescriptor == -1)
    {
    perror("Error while opening customer file");
    return false;
    }
    offset = lseek(customerFileDescriptor, studentID * sizeof(struct student), SEEK_SET);
    if (offset == -1)
    {
    perror("Error while seeking to required student record!");
    return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
    perror("Error while obtaining write lock on student record!");
    return false;
    }

    writeBytes = write(customerFileDescriptor, &stu, sizeof(struct student));
    if (writeBytes == -1)
    {
    perror("Error while writing update student info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);

    close(customerFileDescriptor);

    writeBytes = write(connFD, ADMIN_MOD_SUCCESS, strlen(ADMIN_MOD_SUCCESS));
    if (writeBytes == -1)
    {
    perror("Error while writing ADMIN_MOD_SUCCESS message to client!");
    return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return true;
    }

// Modify Faculty info
bool modify_faculty_details(int connFD){
    ssize_t readBytes, writeBytes;
    char readBuffer[1000], writeBuffer[1000];

    struct faculty fac;

    int facultyId;

    off_t offset;
    int lockingStatus;

    writeBytes = write(connFD, ADMIN_MOD_FACULTY_ID, strlen(ADMIN_MOD_FACULTY_ID));
    if (writeBytes == -1)
    {
    perror("Error while writing ADMIN_MOD_FACULTY_ID message to client!");
    return false;
    }
    bzero(readBuffer, sizeof(readBuffer));
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
    perror("Error while reading faculty ID from client!");
    return false;
    }

    facultyId = atoi(readBuffer);

    int customerFileDescriptor = open(FACULTY_FILE, O_RDONLY);
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
    
    offset = lseek(customerFileDescriptor, facultyId * sizeof(struct faculty), SEEK_SET);
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

    struct flock lock = {F_RDLCK, SEEK_SET, offset, sizeof(struct faculty), getpid()};

    // Lock the record to be read
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
    perror("Couldn't obtain lock on faculty record!");
    return false;
    }

    readBytes = read(customerFileDescriptor, &fac, sizeof(struct faculty));
    if (readBytes == -1)
    {
    perror("Error while reading faculty record from the file!");
    return false;
    }

    // Unlock the record
    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLK, &lock);

    close(customerFileDescriptor);

    writeBytes = write(connFD, ADMIN_MOD_FACULTY_MENU, strlen(ADMIN_MOD_FACULTY_MENU));
    if (writeBytes == -1)
    {
    perror("Error while writing ADMIN_MOD_FACULTY_MENU message to client!");
    return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
    perror("Error while getting faculty modification menu choice from client!");
    return false;
    }

    int choice = atoi(readBuffer);
    if (choice == 0)
    { // A non-numeric string was passed to atoi
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, ERRON_INPUT_FOR_NUMBER);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
    perror("Error while writing ERRON_INPUT_FOR_NUMBER message to client!");
    return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return false;
    }

    bzero(readBuffer, sizeof(readBuffer));
    switch (choice)
    {
    case 1:
    writeBytes = write(connFD, ADMIN_MOD_NEW_NAME, strlen(ADMIN_MOD_NEW_NAME));
    if (writeBytes == -1)
    {
    perror("Error while writing ADMIN_MOD_NEW_NAME message to client!");
    return false;
    }
    readBytes = read(connFD, &readBuffer, sizeof(readBuffer));
    if (readBytes == -1)
    {
    perror("Error while getting response for student's new name from client!");
    return false;
    }
    strcpy(fac.name, readBuffer);
    break;
    
    default:
    bzero(writeBuffer, sizeof(writeBuffer));
    strcpy(writeBuffer, INVALID_MENU_CHOICE);
    writeBytes = write(connFD, writeBuffer, strlen(writeBuffer));
    if (writeBytes == -1)
    {
    perror("Error while writing INVALID_MENU_CHOICE message to client!");
    return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read
    return false;
    }

    customerFileDescriptor = open(FACULTY_FILE, O_WRONLY);
    if (customerFileDescriptor == -1)
    {
    perror("Error while opening faculty file");
    return false;
    }
    offset = lseek(customerFileDescriptor, facultyId * sizeof(struct faculty), SEEK_SET);
    if (offset == -1)
    {
    perror("Error while seeking to required faculty record!");
    return false;
    }

    lock.l_type = F_WRLCK;
    lock.l_start = offset;
    lockingStatus = fcntl(customerFileDescriptor, F_SETLKW, &lock);
    if (lockingStatus == -1)
    {
    perror("Error while obtaining write lock on faculty record!");
    return false;
    }

    writeBytes = write(customerFileDescriptor, &fac, sizeof(struct faculty));
    if (writeBytes == -1)
    {
    perror("Error while writing update faculty info into file");
    }

    lock.l_type = F_UNLCK;
    fcntl(customerFileDescriptor, F_SETLKW, &lock);

    close(customerFileDescriptor);

    writeBytes = write(connFD, ADMIN_MOD_SUCCESS, strlen(ADMIN_MOD_SUCCESS));
    if (writeBytes == -1)
    {
    perror("Error while writing ADMIN_MOD_SUCCESS message to client!");
    return false;
    }
    readBytes = read(connFD, readBuffer, sizeof(readBuffer)); // Dummy read

    return true;
}

#endif