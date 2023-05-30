#include <stdio.h>
#include <string.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
# include <sys/socket.h>



int creating_socket(void){ //Function to create the socket.
return socket (AF_INET,SOCK_STREAM,0);//AF_INET as communication domain , SOCK_STREAM as communication semantics and 0 as the one protocol that can be used.
}



int binding () {  //This function does the binding.
const struct sockaddr * address;
//address->sin_family=AF_INET;



}



