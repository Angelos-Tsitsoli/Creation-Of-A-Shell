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
pthread_cond_t con_v_not_empty ;
pthread_cond_t con_v_not_full ;           //-lpthread
static int buffer_iterator=0;
Hash_table_node * Hash_table;
static int sock;

void * customer (int sock ){
    int result=0;
    char * parties_and_names[15];
    pthread_mutex_lock (&mut);
    printf("Inside thread creation\n");
    while ( buffer_iterator <= 0) {
        printf("before wait not_empty\n");
        pthread_cond_wait (&con_v_not_empty , &mut ) ;
    }
    printf("after wait not_empty\n");
    printf("SEND NAME PLEASE\n");
    //write(sock,"SEND NAME PLEASE",strlen("SEND NAME PLEASE"));
    Worker_action( sock ,Hash_table,Size,"name",parties_and_names);
    printf("GEIA\n");
    result=1;//Search(Hash_table,parties_and_names[0],parties_and_names[1],Size);
    if (result!=1){
        printf("SEND VOTE PLEASE\n");
        //write(sock,"SEND VOTE PLEASE",strlen("SEND VOTE PLEASE"));
        Worker_action ( sock ,Hash_table,Size,"vote",parties_and_names);
        char m[20];
        strcpy(m, "VOTE for Party ");
        strcat(m, parties_and_names[0]);
        strcat(m, " RECORDED");
       // write(sock, m, strlen(m));
        for (int i=0;i<3;i++){
            free(parties_and_names[i]);
        }
      
    }

    if (result==-1){
        printf("ALREADY VOTED\n");
        //write(sock,"ALREADY VOTED",strlen("ALREADY VOTED"));
        for (int i=0;i<3;i++){
            free(parties_and_names[i]);
        }
    }
    pthread_mutex_unlock (&mut);
    

}


void place (int buffer_size,int connect,int *storing_buffer ) {
    pthread_mutex_lock (&mut) ;
    printf("In place first %d \n",buffer_iterator);
    while ( buffer_iterator >= buffer_size ) {
        pthread_cond_wait (&con_v_not_full , &mut ) ;
    }

    storing_buffer[buffer_iterator]=connect;
    buffer_iterator++;
    pthread_mutex_unlock (&mut ) ;
    printf("In place second %d \n",buffer_iterator);
    pthread_cond_signal (&con_v_not_empty);

}

void * customer_caller ( void * ptr )
{
    customer(sock);
    pthread_cond_signal (&con_v_not_full);
    pthread_exit (0) ;
}




int main(/*int argc , char * argv []*/){

int portnum=5634;//atoi(argv[1]);
int numWorkerthreads=8;//atoi(argv[2]);
int bufferSize=16;//atoi(argv[3]);
char *poll_log="";//(argv [4]);
char* poll_stats="";//(argv [5]);
int * storing_buffer=malloc(bufferSize * sizeof(int));




pthread_mutex_init(&mut,NULL); 
pthread_cond_init(&con_v_not_empty,NULL);
pthread_cond_init(&con_v_not_full,NULL);

//cd project2
//gcc -o poller poller.c -lpthread
//172.21.224.1
//telnet localhost 5634
//./poller


/////////////////////////////////////////////////////create socket//////////////////
int socket_fd=creating_socket();
if (socket_fd<0){
    printf("Error creating socket\n");
    return -1;
}
//////////////////////////////////////////////////////////////////////////////////

sock=socket_fd;
///////////////////////////////////////////////////bind////////////////////////////////
int binding_error= binding(socket_fd,portnum);
if (binding_error<0){
    printf("Error in binding socket\n");
    return -1;
}
////////////////////////////////////////////////////////////////////////////////////
printf("ooooooooo %d\n",socket_fd);

////////////////////////////////////////////////////////listen/////////////////////////////
int listening_error=listening(socket_fd,16);
if (listening_error<0){
    printf("Error in listening \n");
    return -1;
}
///////////////////////////////////////////////////////////////////////////////////////


/////////////////////// Threads creation ////////////////////////////////////////////
pthread_t * Thread; //Process of creating the threads
printf("Creating Threads\n");
Thread= malloc( numWorkerthreads * sizeof ( pthread_t ) );
for ( int i =0 ; i < numWorkerthreads ; i ++) {//Creating threads .
    pthread_create( Thread + i , NULL , customer_caller , NULL ); //THELEI FTIAKSIMOOOO 

}
///////////////////////////////////////////////////////////////////


sleep (2) ;
printf("Hello\n");
while(1){

///////////////////////accept////////////////////////////////////////////
int connect=accepting(socket_fd);
if (connect<0){
    printf("Error in accepting \n");
    return -1;
}
printf("I just accepted\n");
printf("Place function about to happen\n");
place(bufferSize,connect,storing_buffer);


///////////////////////////////////////////////////////////////////////////////////////////////



}
free(storing_buffer);
return 0;
}
