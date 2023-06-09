#include <stdio.h>
#include <string.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <pthread.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>






int creating_socket(void){ //Function to create the socket.
    printf("Creating a socket\n");
    return socket (AF_INET,SOCK_STREAM,0);//AF_INET as communication domain , SOCK_STREAM as communication semantics and 0 as the one protocol that can be used.
}


int binding(int the_socket, int port) {
    printf("Binding a socket\n");
    struct sockaddr_in address_of_server;
    address_of_server.sin_family = AF_INET;
    address_of_server.sin_addr.s_addr = htonl(INADDR_ANY);
    address_of_server.sin_port = htons(port);
    return bind(the_socket, (struct sockaddr*)&address_of_server, sizeof(address_of_server));
}

int listening(int the_socket , int waiting_number){
    printf("Listening process\n");
    return listen (the_socket ,waiting_number);
}

int accepting(int the_socket ){
    printf("Accepting process\n");
    struct sockaddr_in address_of_client;
    socklen_t * length;
    socklen_t address_length = sizeof(address_of_client);
    //struct sockaddr * pointer_to_address =( struct sockaddr *) & address_of_client ;
    return accept ( the_socket ,  NULL/*(struct sockaddr*)&address_of_client*/, &address_length); ///////////////////////////SHMANTIKO THELEI NULL TO DEYTERO
}


void Reseting(char * word){   
   // Reset iterator to beginning of word array
    char* iterator3 = word;

    // Loop through word array to count length of string
    int length = 0;
    while (*iterator3 != '\0') {
        length++;
        iterator3++;
    }

    // Clear contents of word array
    for (int i = 0; i < length; i++) {
        word[i] = '\0';
    }

}



