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
#include <structures.h>


pthread_mutex_t mut ;
pthread_cond_t con_v ;


void * sleeping_untill_siganl(){
    pthread_mutex_lock(&mut);
    pthread_cond_wait(&con_v,&mut);
    pthread_mutex_unlock(&mut);
    pthread_exit (NULL);
}


void * signal(){
    pthread_mutex_lock (&mut);
    pthread_cond_signal (&con_v);
    printf("SEND NAME PLEASE\n");
    read ( newsock , buf , 1)
    pthread_mutex_unlock(&mut);
    pthread_exit ( NULL ) ;
}




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
return accept ( the_socket , NULL ,NULL); ///////////////////////////SHMANTIKO THELEI NULL TO DEYTERO
}

static int buffer_iterator=0;

int main(/*int argc , char * argv []*/){

int portnum=5634;//atoi(argv[1]);
int numWorkerthreads=8;//atoi(argv[2]);
int bufferSize=16;//atoi(argv[3]);
char *poll_log="";//(argv [4]);
char* poll_stats="";//(argv [5]);

int * storing_buffer=malloc(bufferSize * sizeof(int));

pthread_mutex_init(&mut,NULL); 
pthread_cond_init(&con_v,NULL);



///////////////////////[     1      ]////////////////////////////////////////////
pthread_t * Thread; //Process of creating the threads
Thread= malloc( numWorkerthreads * sizeof ( pthread_t ) );
for ( int i =0 ; i < numWorkerthreads ; i ++) {//Creating threads .
 pthread_create( Thread + i , NULL , sleeping_untill_siganl , NULL ); //THELEI FTIAKSIMOOOO 

}
///////////////////////////////////////////////////////////////////




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

///////////////////////[     2 kai 3      ]////////////////////////////////////////////
int connect=accepting(socket_fd);
if (connect<0){
    printf("Error in accepting \n");
    return -1;
}

storing_buffer[buffer_iterator]=connect;
buffer_iterator++;
///////////////////////////////////////////////////////////////////////////////////////////////


}









free(storing_buffer);

return 0;
}
