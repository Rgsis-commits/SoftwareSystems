#include <unistd.h> // To include the read and write function calls
#include <stdio.h> //  To use the stdin and stdout functions

#define BUFFER_SIZE 1024
int main(){
	char buffer[BUFFER_SIZE]; // to store the data 
	ssize_t bytes_read, bytes_written; // no of bytes read or written
	while((bytes_read = read(STDIN_FILENO, buffer, sizeof(buffer)))>0){
		bytes_written = write(STDOUT_FILENO, buffer, bytes_read); // doesn't take the other non printable character or the control character from keyboard
		if(bytes_written == -1){
			perror("Error in writting the data");
			return 1;
		}
	}
	if(bytes_read == -1){
		perror("Error in reading the data");
		return 1;
	}
	return 0;
}
