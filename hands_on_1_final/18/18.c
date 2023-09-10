/*
============================================================================
Name : 18.c
Author : Rishav Chandel
Description : Write a program to perform Record locking.
 a. Implement write lock
 b. Implement read lock
Create three records in a file. Whenever you access a particular record, first lock it then modify/access
to avoid race condition.
============================================================================
*/
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<string.h>

int main(int argc, char* argv[]){
        if(argc!=3){
                printf("Required number of arguments not present\n");
                return 0;
        }

        int fd = open("records.txt", O_RDWR, 0777);

        if(fd==-1){
                perror("Could not open the file\n");
                return 0;
        }

        int record = (int)*argv[2] - 48;

        int lock_type;
        if(strcmp(argv[1],"write")==0) lock_type=1;
        else if(strcmp(argv[1],"read")==0)lock_type=2;
        else{
                printf("Please provide valid lock type to be attained\n");
                return 0;
        }



        if(record>=1 && record <=3){

                struct flock fl;

                switch (lock_type){
                        case 1:
                                printf("Before Critical Section\n");
                                fl.l_type = F_WRLCK;
                                fl.l_whence = SEEK_SET;
                                fl.l_start = 10*(record-1);
                                fl.l_len = 10;
                                fl.l_pid = getpid();

                                fcntl(fd, F_SETLKW, &fl);


                                printf("Write lock on record %d attained\n",record);
                                break;
                        case 2:
                                printf("Before Critical Section\n");
                                fl.l_type = F_RDLCK;
                                fl.l_whence = SEEK_SET;
                                fl.l_start = 10*(record-1);
                                fl.l_len = 10;
                                fl.l_pid = getpid();

                                fcntl(fd, F_SETLKW, &fl);

                                printf("Read lock on record %d attained\n",record);
                                break;
                        default:
                                printf("Please provide valid lock type to be attained\n");
                                break;

                }

                printf("\nEnter to release lock\n");
                getchar();
                fl.l_type = F_UNLCK;
                fcntl(fd, F_SETLK, &fl);

                printf("%s Lock Released on record %d\n",argv[1],record);

                close(fd);
        }
}
