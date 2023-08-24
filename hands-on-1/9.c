#include<stdio.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<fcntl.h>
int main(){
	struct stat stat_buf;
	int x = stat("/input_file.txt", &stat_buf);
	if(x != 0){
		printf("Inode number is:%ld\n ", stat_buf.st_ino);
	     	printf("Number of hard links:%ld\n ", stat_buf.st_nlink);
		printf("User id is:%d\n ", stat_buf.st_uid);
		printf("group user id :%d\n ", stat_buf.st_gid);
		printf("size of file :%ld\n ", stat_buf.st_size);
		printf("block size:%ld\n ", stat_buf.st_blksize);
		printf("number of blocks:%ld\n ", stat_buf.st_blocks);
		printf("time of last access: %ld\n ", stat_buf.st_atim.tv_sec);
		printf("time of last modification:%ld\n ", stat_buf.st_mtim.tv_sec);
		printf("time of last change:%ld\n ", stat_buf.st_ctim.tv_sec);	
	}else{
		printf("invalid: ");
	}
	
}
