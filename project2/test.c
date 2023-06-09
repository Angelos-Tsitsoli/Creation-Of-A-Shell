#include <stdio.h>
#include <string.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>


int main() {
	if ( fork() && fork() ) 
		printf("hello\n");
	else
		printf("world\n");
	fork() ? printf("hello\n") : printf("world\n");
	return 0;
}




