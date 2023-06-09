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
#define SIZE 90

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
    printf("The result of placing func %d\n",(num1 + 1)%(num2));
    return (num1 + 1)%(num2);

}


void making_sure_write_sends(int socket, char* buffer, size_t bufferSize){
    //printf("Writing\n");
    size_t bytessent=0;
    size_t byte;
    //printf("HERE\n");
    size_t headerSize = sizeof(bufferSize);
    size_t bufferSizeNetwork = htonl(bufferSize);
    printf("Writing size of bytes in :%d \n",socket);
    size_t bytesSent = write(socket, &bufferSizeNetwork, headerSize);
    //printf("FINISH\n");

    printf("Writing the data in :%d \n",socket);
    while (bytessent < bufferSize) {
           byte =write(socket, buffer + bytessent, bufferSize - bytessent);

           if (byte == -1) {
               perror("write");
               break;
           }

           bytessent += byte;
       }
   printf("End of writing %ld \n",bytessent); 
}


/////////////////////////////////////////////////////
void place (the_buffer* buff,int file_des) {
    pthread_mutex_lock (&mut) ;
    while (buff->counter>=size){
        printf ( "The buffer is full , waiting is needed , untill not full\n" ) ;
        pthread_cond_wait (&con_v_not_full,&mut) ;
    }
    printf("In place %d\n",file_des);
    
    buff->rear=( buff->rear + 1) % size; //placing_func(buff->rear,size);//ftiakse mia synarthsh
    printf("I placed %d in place %d\n",file_des,buff->rear);
    buff->fds[buff->rear]=file_des;
    buff->counter++;
    pthread_mutex_unlock (&mut);
    pthread_cond_signal (&con_v_not_empty) ;

}
//////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
void producer ( the_buffer* buff,int file_des)
{
    printf("About to place an item to the buffer, the->%d\n",file_des);
    place (buff,file_des);
    printf("A file descriptor has just been added\n");
    
}
//////////////////////////////////////////////////////


void making_sure_read(int socket, char* buffer) {

    //printf("Hello\n");
    size_t messageSize2;
    printf("Reading size of bytes in :%d \n",socket);
    size_t bytesRead1 = read(socket, &messageSize2, sizeof(messageSize2));

    if (bytesRead1 != sizeof(messageSize2)) {
        perror("read");
        return;
    }

    messageSize2 = ntohl(messageSize2);  // Convert message size to host byte order

    printf("Message size: %zu\n", messageSize2);



    //printf("Reading\n");
    size_t bytesReceived = 0;
    size_t bytesRead;

    printf("Reading the data in :%d \n",socket);
    while (bytesReceived < messageSize2) {
        bytesRead = read(socket, buffer + bytesReceived, messageSize2 - bytesReceived);

        if (bytesRead == -1) {
            perror("read");
            break;
        } else if (bytesRead == 0) {
            // Connection closed by the server
            break;
        }

        bytesReceived += bytesRead;
    }

    printf("End of reading: %ld bytes received\n", bytesReceived);
    
}




void Worker_action(name_surname_politicalparty* nspp, int the_socket, int a_case, char* buffer) {
    char array[70];
    int flag=0;
    int j;
    //printf("In worker action\n");
    printf("%ld\n", strlen(buffer));
    //printf("The buffer ->%s\n", buffer);
    Reseting(array);
    for (int i = 0; i < strlen(buffer); i++) {
       // printf("bufr -> %c i:%d\n", buffer[i],i);
        if (a_case == 0) {
            //printf("IN a_case=0\n");
            if (buffer[i] == ' ') {
                //printf("Symbol space \n");
                strcpy(nspp->name, array);
                //printf("In worker and found space symbol : array %s nspp%s\n",array,nspp->name);               
                flag=1;
                Reseting(array);
            }
           
        }

        
        if (flag==1){
            array[j] = buffer[i];
        }

        if (flag==0){
            array[i] = buffer[i];
        }
        //printf("In a_case 1 %c", array[i]);
    }


    if (a_case == 0) {
        strcpy(nspp->surname, array);
    }

    if (a_case == 1) {  
        strcpy(nspp->politicalparty, array);
    }
    
    //array[i] = '\0'; // Add null terminator to treat array as a string
    //printf("Name in worker: %s\n", array);
}

///////////////////////////////////////////////////
int obtain (the_buffer * buffer  ) {
    pthread_mutex_lock (&mut);
    while ( buffer->counter <= 0) {
        printf ( "The buffer is empty , waiting is needed , untill not empty\n" ) ;
        pthread_cond_wait (&con_v_not_empty,&mut ) ;
    }

    
    int desc=buffer->fds[buffer->front ];
    printf("I took %d from place %d\n",desc,buffer->front);
    buffer->front=(buffer->front + 1) % size ;//placing_func(buffer->front,size);
    printf("New position of front (%d) tis ekato (%d)\n",buffer->front + 1,size);
    buffer->counter--;
    pthread_mutex_unlock (&mut) ;
    pthread_cond_signal (&con_v_not_full) ;
    return desc ;
}
////////////////////////////////////////////////////



void * consumer ( void * ptr )
{
    //OLO AYTO SE WHILE 
    //sleep(5);
    char str1[20]= "SEND NAME PLEASE";
    char str2[20] = "SEND VOTE PLEASE"; 
    char str3[20] = "ALREADY VOTED"; 


    char* buffer = malloc(18);
    char* buffer2 = malloc(18);

    name_surname_politicalparty *nspp = malloc(sizeof(name_surname_politicalparty));

    int result=obtain(&buff);
    
    //sleep(1);
    printf("#1 Writing in :%d \n",result);
    making_sure_write_sends(result, str1, (size_t)strlen(str1));//1
    
    printf("#2 Reading in :%d \n",result);
    making_sure_read(result,buffer);//2
    Worker_action(nspp,result,0, buffer);
    //printf("ELAAA ->%s\n",nspp->name);
    pthread_mutex_lock (&mut);
    int returning_num=Search(Hash_table,nspp->name ,SIZE);
    pthread_mutex_unlock (&mut);

    if (returning_num==-1){
        printf("#3 Writing in :%d \n",result);
        making_sure_write_sends(result, str2, (size_t)strlen(str2));//3 PLEASE VOTE
        printf("#4 Reading in :%d \n",result);
        making_sure_read(result,buffer2);//4
        Worker_action(nspp,result,1, buffer2);
        char m[20];
        strcpy(m, "VOTE for Party " );
        strcat(m, nspp->politicalparty);
        strcat(m, " RECORDED");
        printf("#5 Writing in :%d \n",result);
        making_sure_write_sends(result, m, (size_t)strlen(m));//5
        pthread_mutex_lock (&mut);
        Insert(Hash_table,nspp->name,nspp->surname,nspp->politicalparty,SIZE);
        pthread_mutex_unlock (&mut);
        close(result);
    }
    if (returning_num!=-1){
        printf("#5 Writing in (enalaktiko):%d \n",result);
        making_sure_write_sends(result, str3, (size_t)strlen(str3));
        close(result);
    }
    //printf("After \n");
    free(nspp);
    free(buffer);
    free(buffer2);
    pthread_exit(0);
}




int main(/*int argc , char * argv []*/){

    int portnum=5634;//atoi(argv[1]);
    int numWorkerthreads=8;//atoi(argv[2]);
    int bufferSize=16;//atoi(argv[3]);
    char *poll_log="";//(argv [4]);
    char* poll_stats="";//(argv [5]);

    size=bufferSize;

    Hash_table = malloc(SIZE * sizeof(Hash_table_node));
    Initialization(Hash_table,size);
    
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



    printf("I just accepted a connection ->%d\n",connect);

    //printf("connection %d\n",connect);
    producer(&buff,connect);
    printf("After place\n");
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
