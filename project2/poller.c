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
#include "structures.h"
#define Size 90

pthread_mutex_t mut ;
pthread_cond_t con_v_empty ;
pthread_cond_t con_v_full ;


void * customer (int sock, int buffer_sofar , int size ,Hash_table_node * Hash_table ){
int result=0;
char * parties_and_names[15];
pthread_mutex_lock (&mut);
while ( buffer_sofar <= 0) {
    pthread_cond_wait (&con_v_empty , &mut ) ;
}

write(sock,"SEND NAME PLEASE",strlen("SEND NAME PLEASE"));
Worker_action( sock ,Hash_table,size,"name",parties_and_names);
//result=Search();
for (int i=0;i<2;i++){
    free(parties_and_names[i]);
}
if (result==0){
    write(sock,"SEND VOTE PLEASE",strlen("SEND VOTE PLEASE"));
    Worker_action ( sock ,Hash_table,size,"vote",parties_and_names);
    char m[20];
    strcpy(m, "VOTE for Party ");
    strcat(m, parties_and_names[0]);
    strcat(m, " RECORDED");
    write(sock, m, strlen(m));
    free(parties_and_names[0]);
}

if (result==1){
    write(sock,"ALREADY VOTED",strlen("ALREADY VOTED"));
    pthread_mutex_unlock (&mut);
    pthread_cond_signal (&con_v_empty);
    free(parties_and_names[0]);
}

}


void place (int buffer_sofar , int buffer_size ) {
pthread_mutex_lock (&mut) ;
while ( buffer_sofar >= buffer_size ) {
    pthread_cond_wait (&con_v_full , &mut ) ;
}

//storing_buffer[buffer_iterator]=connect;
//buffer_iterator++;

pthread_mutex_unlock (&mut ) ;
pthread_cond_signal (&con_v_full) ;
}



static int buffer_iterator=0;

int main(/*int argc , char * argv []*/){

int portnum=5634;//atoi(argv[1]);
int numWorkerthreads=8;//atoi(argv[2]);
int bufferSize=16;//atoi(argv[3]);
char *poll_log="";//(argv [4]);
char* poll_stats="";//(argv [5]);
int * storing_buffer=malloc(bufferSize * sizeof(int));
Hash_table_node * Hashing_table;



pthread_mutex_init(&mut,NULL); 
pthread_cond_init(&con_v_empty,NULL);
pthread_cond_init(&con_v_full,NULL);




/////////////////////////////////////////////////////create socket//////////////////
int socket_fd=creating_socket();
if (socket_fd<0){
    printf("Error creating socket\n");
    return -1;
}
//////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////bind////////////////////////////////
int binding_error= binding(socket_fd,portnum);
if (binding_error<0){
    printf("Error in binding socket\n");
    return -1;
}
////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////listen/////////////////////////////
int listening_error=listening(socket_fd,16);
if (listening_error<0){
    printf("Error in listening \n");
    return -1;
}
///////////////////////////////////////////////////////////////////////////////////////


/////////////////////// Threads creation ////////////////////////////////////////////
pthread_t * Thread; //Process of creating the threads
Thread= malloc( numWorkerthreads * sizeof ( pthread_t ) );
for ( int i =0 ; i < numWorkerthreads ; i ++) {//Creating threads .
    pthread_create( Thread + i , NULL , customer(socket_fd,buffer_iterator, Size , Hashing_table ) , NULL ); //THELEI FTIAKSIMOOOO 

}
///////////////////////////////////////////////////////////////////




while(1){

///////////////////////accept////////////////////////////////////////////
int connect=accepting(socket_fd);
if (connect<0){
    printf("Error in accepting \n");
    return -1;
}


place (buffer_iterator,bufferSize);


///////////////////////////////////////////////////////////////////////////////////////////////



}


free(storing_buffer);

return 0;
}
