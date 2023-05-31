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


int binding (int the_socket,int port) {  //This function does the binding.
struct sockaddr_in  address_of_server ;
//struct sockaddr * pointer_to_address =( struct sockaddr *) & address_of_server ;
address_of_server.sin_family=AF_INET;
address_of_server.sin_addr.s_addr=htonl(INADDR_ANY);
address_of_server.sin_port=htons(port);
return bind (socket,(struct sockaddr*)&address_of_server,sizeof(address_of_server)) ;
}

int listening(int the_socket , int waiting_number){
return listen (the_socket ,waiting_number);
}

int accepting(int the_socket ){
struct sockaddr_in address_of_client;
socklen_t * length;
//struct sockaddr * pointer_to_address =( struct sockaddr *) & address_of_client ;
return accept ( the_socket , (struct sockaddr*)&address_of_client ,&length);
}





int main(){
return 0;
}
