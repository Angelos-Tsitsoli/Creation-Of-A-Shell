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
#define SIZE 16

///////////////////////////// The shared buffer/////////////
typedef struct {
    int* fds;
    int front ;
    int rear ;
    int counter ;
} the_buffer ;

the_buffer buff;
///////////////////////////////////////////////////////////

////////////////////////////// A vote //////////////////
typedef struct {
    char name[20];
    char surname[20];
    char politicalparty[25];
} name_surname_politicalparty ;
//////////////////////////////////////////////////////////////




//////////////// threads mutex//////////////////////////
pthread_mutex_t mut ;
pthread_cond_t con_v_not_empty ;
pthread_cond_t con_v_not_full ;           
////////////////////////////////////////////////

Hash_table_node * Hash_table;
static int size;


void Initial_buff(the_buffer * buffer ){
    buffer-> front = 0;
    buffer-> rear = -1;
    buffer-> counter = 0;
}

int placing_func(int num1 , int num2){
    return (num1 + 1)%(num2);

}


void making_sure_write_sends(int socket, char* buffer, size_t bufferSize){

    size_t bytessent=0;
    size_t byte;
    while (bytessent < bufferSize) {
           byte =write(socket, buffer + bytessent, bufferSize - bytessent);

           if (byte == -1) {
               perror("write");
               break;
           }

           bytessent += byte;
       }

}


/////////////////////////////////////////////////////
void place (the_buffer* buff,int file_des) {
    pthread_mutex_lock (&mut) ;
    while (buff->counter>=size){
        printf ( "The buffer is full , waiting is needed , untill not full\n" ) ;
        pthread_cond_wait (&con_v_not_full,&mut) ;
    }
    buff->rear=placing_func(buff->rear,size);//ftiakse mia synarthsh
    buff->fds[buff->rear]=file_des;
    buff->counter++;
    pthread_mutex_unlock (&mut);
}
//////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
void producer ( the_buffer* buff,int file_des)
{
    printf("About to place an item to the buffer\n");
    place (buff,file_des);
    printf("A file descriptor has just been added\n");
    pthread_cond_signal (&con_v_not_empty) ;

}
//////////////////////////////////////////////////////


void Worker_action(name_surname_politicalparty * nspp, int the_socket , char * a_case ) {
char single_char[1];
char array [70];
int i=0;
while (read( the_socket , single_char , 1) > 0) { //PROSOXH APLA OTAN GRAFEIS ONOMA THA PRPEEI NA MHN BAZEIS SPACE STO TELOS AYTA
  if(strcmp(a_case,"name")==0){
    if(single_char[0]==' '){
      strcpy(nspp->name,array);
      i=0;
      Reseting(array);
    }
    if(single_char[0]=='\n'){
      strcpy(nspp->surname,array);
      break;
    }
  }

  if(strcmp(a_case,"vote")==0){
    strcpy(nspp->politicalparty,array);
    break;
  }
    array[i]=single_char[0];
    i++;
}

}

///////////////////////////////////////////////////
int obtain (the_buffer * buffer  ) {
    pthread_mutex_lock (&mut);
    while ( buffer->counter <= 0) {
        printf ( "The buffer is empty , waiting is needed , untill not empty\n" ) ;
        pthread_cond_wait (&con_v_not_empty,&mut ) ;
    }

    int desc=buffer->fds[buffer->front ];
    buffer->front=placing_func(buffer->front,size);
    buffer->rear--;
    pthread_mutex_unlock (&mut) ;
    return desc ;
}
////////////////////////////////////////////////////



void * consumer ( void * ptr )
{
    //OLO AYTO SE WHILE 
    name_surname_politicalparty nspp;
    int result=obtain (&buff);
    
    write(result,"SEND NAME PLEASE :",strlen("SEND NAME PLEASE"));
    Worker_action(&nspp,result,"name");


    int returning_num=Search(Hash_table,nspp.name ,SIZE);
     if (returning_num!=1){
        write(result,"SEND VOTE PLEASE",sizeof("SEND VOTE PLEASE"));
        Worker_action(&nspp,result,"party");
        char m[20];
        strcpy(m, "VOTE for Party ");
        strcat(m, nspp.politicalparty);
        strcat(m, " RECORDED");
        write(result, m, strlen(m));
        close(result);
    }
    if (returning_num==-1){
        write(result,"ALREADY VOTED",strlen("ALREADY VOTED"));
        close(result);
    }


    pthread_cond_signal (&con_v_not_full) ;
    
    pthread_exit (0) ;
}







int main(/*int argc , char * argv []*/){

    int portnum=5634;//atoi(argv[1]);
    int numWorkerthreads=8;//atoi(argv[2]);
    int bufferSize=16;//atoi(argv[3]);
    char *poll_log="";//(argv [4]);
    char* poll_stats="";//(argv [5]);

    size=bufferSize;

    
    /////////////////////////////////Initialization////////////////////
    pthread_mutex_init(&mut,NULL); 
    pthread_cond_init(&con_v_not_empty,NULL);
    pthread_cond_init(&con_v_not_full,NULL);
    //////////////////////////////////////////////////////


    ////////////// Initialization of the buffer //////////////////////////
    buff.fds=(int*)malloc(bufferSize * sizeof(int));//ARGV!!!!!!
    Initial_buff(&buff);
    ////////////////////////////////////////////////////////////////


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
    printf("Creating Threads\n");
    Thread= malloc( numWorkerthreads * sizeof ( pthread_t ) );
    for ( int i =0 ; i < numWorkerthreads ; i ++) {//Creating threads .
        pthread_create( Thread + i , NULL , consumer , (void*)&buff ); //THELEI FTIAKSIMOOOO 

    }
    ///////////////////////////////////////////////////////////////////


    while(1){
    printf("About to accept a connection\n");
    ///////////////////////accept////////////////////////////////////////////
    int connect=accepting(socket_fd);
    if (connect<0){
        printf("Error in accepting \n");
        return -1;
    }



    printf("I just accepted a connection\n");

    producer(&buff,connect);

    ///////////////////////////////////////////////////////////////////////////////////////////////

    }



    /////////////////////Waiting the threads////////////////
    for ( int i =0 ; i < numWorkerthreads ; i ++) {
       pthread_join ( Thread[i] , 0) ;
    }
    ////////////////////////////////////////////////////

    ///////////////////////Destruction/////////////////////////////////////////
    pthread_cond_destroy (&con_v_not_empty ) ;
    pthread_cond_destroy (&con_v_not_full ) ;
    pthread_mutex_destroy (&mut ) ;
    ///////////////////////////////////////////////////////////////////


    ////////////Free memory//////////////////////
    free(Thread);
    free(buff.fds);
    /////////////////////////////////////////


    return 0;
}
