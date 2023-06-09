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
Hash_table_node * Hash_table_for_parties;
/////////////////////////////////Initialization of buffer////////////
void Initial_buff(the_buffer * buffer ){
    buffer-> front = 0;
    buffer-> rear = -1;
    buffer-> counter = 0;
}
/////////////////////////////////////////////

///////////////////////////Function to find a position in buffer to put///////////////////
int placing_func(int num1 , int num2){
    printf("The result of placing func %d\n",(num1 + 1)%(num2));
    return (num1 + 1)%(num2);
}
//////////////////////////////////////////////////////////////////

///////////////////////////////Function to write////////////////////////////////
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
////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////
void Put (the_buffer* buff,int file_des) {
    pthread_mutex_lock (&mut) ;
    while (buff->counter>=size){
        printf ( "The buffer is full , waiting is needed , untill not full\n" ) ;
        pthread_cond_wait (&con_v_not_full,&mut) ;
    }
    printf("In place %d\n",file_des);
    
    buff->counter++;
    buff->rear=placing_func(buff->rear,size);
    //printf("I placed %d in place %d\n",file_des,buff->rear);
    buff->fds[buff->rear]=file_des;
    pthread_mutex_unlock (&mut);
    pthread_cond_signal (&con_v_not_empty) ;

}
//////////////////////////////////////////////////////////////



/////////////////////////////////////////////////
void Provider ( the_buffer* buff,int file_des)
{
    printf("About to place an item to the buffer, the->%d\n",file_des);
    Put(buff,file_des);
    printf("A file descriptor has just been added\n");
    
}
//////////////////////////////////////////////////////

//////////////////////////////////////Function to read//////////////////////////////
void making_sure_read(int socket, char* buffer) {

    
    size_t messageSize2;
    printf("Reading size of bytes in :%d \n",socket);
    size_t bytesRead1 = read(socket, &messageSize2, sizeof(messageSize2));

    if (bytesRead1 != sizeof(messageSize2)) {
        perror("read");
        return;
    }

    messageSize2 = ntohl(messageSize2);  // Convert message size to host byte order

    printf("Message size: %zu\n", messageSize2);



    
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
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////Assigning the message we got from client/////////////////////////
void Assigning(name_surname_politicalparty* nspp, int the_socket, int a_case, char* buffer) {
    char array[70];
    int flag=0;
    int j=0;
    printf("The buffer ->%s\n", buffer);
    for (int i = 0; i < strlen(buffer); i++) {
        if (a_case == 0) {
            if (buffer[i] == ' ') {
                array[i]='\0';
                strcpy(nspp->name, array);            
                flag=1;
                Reseting(array);
            } 
        }
   
        if (flag==1){
            array[j] = buffer[i];
            j++;
        }

        if (flag==0){
            array[i] = buffer[i];
        }  
    }

    if (a_case == 0) {
        array[j]='\0';
        strcpy(nspp->surname, array);
        printf("The nspp->%s,The nspp ->%s\n", nspp->name,nspp->surname);
    }

    if (a_case == 1) { 
        array[strlen(buffer)]='\0'; 
        strcpy(nspp->politicalparty, array);
        printf("The nspp->%s\n", nspp->politicalparty);
    }
    
}
/////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////
int Get (the_buffer * buffer  ) {
    pthread_mutex_lock (&mut);
    while ( buffer->counter <= 0) {
        printf ( "The buffer is empty , waiting is needed , untill not empty\n" ) ;
        pthread_cond_wait (&con_v_not_empty,&mut ) ;
    }

    int desc=buffer->fds[buffer->front];
    buffer->counter=buffer->counter -1;
    //printf("I took %d from place %d\n",desc,buffer->front);
    buffer->front=/*(buffer->front + 1) % size ;*/placing_func(buffer->front,size);
    //printf("New position of front (%d) tis ekato (%d)\n",buffer->front + 1,size);
    pthread_mutex_unlock (&mut) ;
    pthread_cond_signal (&con_v_not_full) ;
    return desc ;
}
////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////
void * Purchaser ( void * ptr )
{
    //OLO AYTO SE WHILE 
    char str1[20]= "SEND NAME PLEASE";
    char str2[20] = "SEND VOTE PLEASE"; 
    char str3[20] = "ALREADY VOTED"; 


    char* buffer = malloc(18);
    char* buffer2 = malloc(18);

    name_surname_politicalparty *nspp = malloc(sizeof(name_surname_politicalparty));

    int result=Get(&buff);
    

    printf("#1 Writing in :%d \n",result);
    making_sure_write_sends(result, str1, (size_t)strlen(str1));//1
    
    printf("#2 Reading in :%d \n",result);
    making_sure_read(result,buffer);//2

    Assigning(nspp,result,0, buffer);

    pthread_mutex_lock (&mut);
    int returning_num=Search_in_hash(Hash_table,nspp->name ,nspp->surname,SIZE);
    pthread_mutex_unlock (&mut);

    if (returning_num==-1){
        printf("#3 Writing in :%d \n",result);
        making_sure_write_sends(result, str2, (size_t)strlen(str2));//3 PLEASE VOTE

        printf("#4 Reading in :%d \n",result);
        making_sure_read(result,buffer2);//4

        Assigning(nspp,result,1, buffer2);

        char m[20];
        strcpy(m, "VOTE for Party " );
        strcat(m, nspp->politicalparty);
        strcat(m, " RECORDED");
        printf("#5 Writing in :%d \n",result);
        making_sure_write_sends(result, m, (size_t)strlen(m));//5

        pthread_mutex_lock (&mut);
        Inserting_to_hash(Hash_table,nspp->name,nspp->surname,nspp->politicalparty,SIZE);
        pthread_mutex_unlock (&mut);

        //printf("\n");
        //printHashTable(Hash_table, SIZE);
        close(result);
    }
    if (returning_num!=-1){
        printf("#5 Writing in (enalaktiko):%d \n",result);
        making_sure_write_sends(result, str3, (size_t)strlen(str3));
        close(result);
    }
    
    free(nspp);
    free(buffer);
    free(buffer2);
    pthread_exit(0);
}
////////////////////////////////////////////////////////////////////////////////////////////



int main(int argc , char * argv []){

    ///////////////////////////////ARGV//////////////////////
    int portnum=atoi(argv[1]);
    int numWorkerthreads=atoi(argv[2]);
    int bufferSize=atoi(argv[3]);
    char *poll_log="";//(argv [4]);
    char* poll_stats="";//(argv [5]);
    /////////////////////////////////////////////////////////

    ///////////KEEPING THE SIZE OF THE BUFFER TO THE VARIABLE 'size'AND THE INITIALIZATION OF HASH TABLE////////////////////////////////////////
    size=bufferSize;  
    Hash_table = malloc(SIZE * sizeof(Hash_table_node));
    Initialization(Hash_table,bufferSize);
    Hash_table_for_parties= malloc(14 * sizeof(Hash_table_node));
    Initialization_for_parties(Hash_table_for_parties,14);
    /////////////////////////////////////////////////////////

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
        pthread_create( Thread + i , NULL , Purchaser , (void*)&buff ); //THELEI FTIAKSIMOOOO 

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
    ////////////////////////////////////////////////////////////////////////////    


    ///////////////////////////////PRODUCER////////////////////////////////////////////
    printf("I just accepted a connection ->%d\n",connect);
    Provider(&buff,connect);
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
