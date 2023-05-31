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
return bind (the_socket,(struct sockaddr*)&address_of_server,sizeof(address_of_server)) ;
}

int listening(int the_socket , int waiting_number){
return listen (the_socket ,waiting_number);
}

int accepting(int the_socket ){
struct sockaddr_in address_of_client;
socklen_t * length;
//struct sockaddr * pointer_to_address =( struct sockaddr *) & address_of_client ;
return accept ( the_socket , (struct sockaddr*)&address_of_client ,length);
}



int main(/*int argc , char * argv []*/){

int portnum=5634;//atoi(argv[1]);
int numWorkerthreads=8;//atoi(argv[2]);
int bufferSize=16;//atoi(argv[3]);
char *poll_log="";//(argv [4]);
char* poll_stats="";//(argv [5]);


int socket_fd=creating_socket();
if (socket_fd<0){
    printf("Error creating socket\n");
    return -1;
}


int binding_error= binding(socket_fd,portnum);
if (binding_error<0){
    printf("Error in binding socket\n");
    return -1;
}


int listening_error=listening(socket_fd,16);
if (listening_error<0){
    printf("Error in listening \n");
    return -1;
}



while(1){

int connect=accepting(socket_fd);
if (connect<0){
    printf("Error in accepting \n");
    return -1;
}





}



for ( i =0 ; i < n ; i ++) {
sl = random () % MAX_SLEEP + 1; /* Sleeping time 1.. MAX_SLEEP */
if ( err = pthread_create ( tids +i , NULL , sleeping , ( void *) sl ) ) {
/* Create a thread */
perror2 ( " pthread_create " , err ) ;
exit (1) ;
}
}







return 0;
}
