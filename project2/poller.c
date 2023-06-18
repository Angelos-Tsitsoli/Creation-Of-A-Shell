#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "structures.h"
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <glob.h>
#include <errno.h>

#define SIZE 90

int flag=0;
FILE *file ;

////////////Flag/////////////
int signal_flag=0;
////////////////////////////


///////////////////////////// The shared buffer/////////////
typedef struct {
    int* fds;
    int front ;
    int rear ;
    int the_iterator ;
} the_buffer ;

the_buffer buff;
///////////////////////////////////////////////////////////

////the files//////////////
char poll_log[25];
char poll_stats[25];
///////////////////////////

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

/////////////Hash tables and size of buffer/////////////////
Hash_table_node * Hash_table;
static int size;
Hash_table_party_node * Hash_table_for_parties;
//////////////////////////////////////////////////

/////////////////////////////////Initialization of buffer////////////

void Start_finish(the_buffer * storage ){
    storage-> front = 0;
    storage-> rear = -1;
}

void First_buff(the_buffer * storage ){
    storage-> the_iterator = 0;
    Start_finish(storage);
}
/////////////////////////////////////////////


void   Handling_the_si (){
    printf("About to handle the signal\n");
    signal_flag=1;
}




///////////////////////////Function to find a position in buffer to put///////////////////
int Calc(int num1 , int num2){
    return num1 % num2; 
}

int placing_func(int num1 , int num2){
    int temp1=num1+1;
    int temp2=num2;
    int temp3=Calc(temp1,temp2);
    return temp3;
}
//////////////////////////////////////////////////////////////////

///////////////////////////////Function to write////////////////////////////////

size_t func_wr(int s,size_t  siz,void * b_siz){
    return write(s, b_siz, siz);
}

int er(size_t error){
    if (error==-1){
        printf("Error in writing\n");
    }
    return error;
}

size_t calc(size_t* bytessent , size_t byte ,size_t bufferSize){

    *bytessent=*bytessent + byte;
    return bufferSize - *bytessent;
}



void making_sure_write_sends(int socket, char* buffer, size_t bufferSize){
    size_t bytessent=0;
    size_t byte;
    size_t headerSize = sizeof(bufferSize);
    size_t bufferSizeNetwork = htonl(bufferSize);
    size_t sent_untill_now;
    size_t bytesSent=func_wr(socket,headerSize, &bufferSizeNetwork);

    //size_t bytesSent = write(socket, &bufferSizeNetwork, headerSize);
    while (bytessent < bufferSize) {
           byte =write(socket, buffer + bytessent, bufferSize - bytessent);

           if(er(byte)==-1){
                break;
           } 
           
           calc(&bytessent , byte , bufferSize);
       }
   printf("End of writing %ld \n",bytessent); 
   printf("I just wrote:%s\n",buffer);
}
////////////////////////////////////////////////////////////////////////////////


void Necess(the_buffer* buff,int file_des){
    buff->rear=placing_func(buff->rear,size);
    buff->fds[buff->rear]=file_des;
}




void Necessary_actions(the_buffer* buff,int file_des){
    buff->the_iterator=buff->the_iterator+1;
    Necess(buff,file_des);
}



////////////////////////Function to place a file descriptor in the buffer /////////////////////////////
void Put (the_buffer* buff,int file_des) {
    pthread_mutex_lock (&mut) ;
    while (buff->the_iterator>=size){
        printf ( "The buffer is full , waiting is needed , untill not full\n" ) ;
        pthread_cond_wait (&con_v_not_full,&mut) ;
    }
  
    Necessary_actions(buff,file_des);
    
    pthread_mutex_unlock (&mut);
    pthread_cond_signal (&con_v_not_empty) ;

}
//////////////////////////////////////////////////////////////



////////////////////This function calls the Put function to place a file descriptor  /////////////////////////////
void Provider ( the_buffer* buff,int file_des)
{
    printf("About to place an item to the buffer, the->%d\n",file_des);
    Put(buff,file_des);
    printf("A file descriptor has just been added\n");
    
}
//////////////////////////////////////////////////////

//////////////////////////////////////Function to read//////////////////////////////

size_t calc2(size_t* bytessent , size_t byte ,size_t bufferSize){

    *bytessent=*bytessent + byte;
    return bufferSize - *bytessent;
}


void making_sure_read(int socket, char* buffer) {

    
    size_t messageSize2;
    printf("Reading size of bytes in :%d \n",socket);
    size_t bytesRead1 = read(socket, &messageSize2, sizeof(messageSize2));

    messageSize2 = ntohl(messageSize2);  // Convert message size to host byte order

    printf("Message size: %zu\n", messageSize2);

    size_t bytesReceived = 0;
    size_t bytesRead;

    printf("Reading the data in :%d \n",socket);
    
    while (bytesReceived < messageSize2) {
        printf("Bytereceive:%ld\n",bytesReceived);
        bytesRead = read(socket, buffer + bytesReceived, messageSize2 - bytesReceived);

        if (bytesRead == -1) {
            perror("read");
            break;
        } else if (bytesRead == 0) {
            printf("Error \n");// Connection closed by the server
            break;
        } 
        calc(&bytesReceived , bytesRead , messageSize2);
    }

    printf("End of reading: %ld bytes received\n", bytesReceived);
    buffer[bytesReceived]='\0';
    printf("I just read:%s\n",buffer);
}
////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////Assigning the message we got from client/////////////////////////
void Assigning(name_surname_politicalparty* nspp, int the_socket, int a_case, char* buffer) {
    char array[70];
    int flag=0;
    int j=0;
    
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


/////////////////////////////The necessay actions for the shared buffer///////////////
int Necessary_actions2(the_buffer * buffer){
    int desc=buffer->fds[buffer->front];
    buffer->the_iterator=buffer->the_iterator -1;
    
    buffer->front=placing_func(buffer->front,size);

    return desc;

}
////////////////////////////////////////////////////////////////////////


////////////////////Taking a file descriptor out///////////////////////////////
int Get (the_buffer * buffer  ) {
    pthread_mutex_lock (&mut);
    while ( buffer->the_iterator <= 0&&signal_flag!=1) {     
        printf ( "The buffer is empty , waiting is needed , untill not empty %d\n",buffer->the_iterator ) ;
        pthread_cond_wait(&con_v_not_empty,&mut ) ;     
    }
    if(signal_flag==1){
        pthread_mutex_unlock (&mut) ;
        return -5;
    }  
    int the_return=Necessary_actions2(buffer); 
    pthread_mutex_unlock (&mut) ;
    pthread_cond_signal (&con_v_not_full) ;
    return the_return ;
}
////////////////////////////////////////////////////


void Necessary_actions3(int result, char* str1,size_t bufferSize,char* buffer, name_surname_politicalparty* nspp,int a_case){

    printf("#1 Writing in :%d \n",result);
    making_sure_write_sends(result, str1, (size_t)strlen(str1));//  
    printf("#2 Reading in :%d \n",result);
    making_sure_read(result,buffer);//  
    Assigning(nspp,result,a_case, buffer);

}



////////////////////////////This function is used by thread workers in order to do whats neccessary/////////////////////////////////////////
void * Purchaser ( void * ptr )
{
    char* buffer;
    char* buffer2;
    name_surname_politicalparty *nspp;
    int result;
    char str1[20]= "SEND NAME PLEASE";
    char str2[20] = "SEND VOTE PLEASE"; 
    char str3[20] = "ALREADY VOTED"; 
    while(signal_flag!=1){
        

        buffer = malloc(50);
        buffer2 = malloc(50);
       
        
        nspp= malloc(sizeof(name_surname_politicalparty));

        
        result=Get(&buff);

        if(result==-5&&signal_flag){
            break;
        }


        Necessary_actions3(result,str1,(size_t)strlen(str1),buffer,nspp,0);

        
        pthread_mutex_lock (&mut);
        int returning_num=Search_in_hash(Hash_table,nspp->name ,nspp->surname,SIZE);
        pthread_mutex_unlock (&mut);
        

        if (returning_num==-1){


            Necessary_actions3(result,str2,(size_t)strlen(str2),buffer2,nspp,1);


            int the_position=Search_in_hash_party(Hash_table_for_parties,nspp->politicalparty ,16);
            Hash_table_for_parties[the_position].votes++;

            char m[20];
            strcpy(m, "VOTE for Party " );
            strcat(m, nspp->politicalparty);
            strcat(m, " RECORDED");
            printf("#5 Writing in :%d \n",result);
            making_sure_write_sends(result, m, (size_t)strlen(m));//5
            pthread_mutex_lock (&mut);
            Inserting_to_hash(Hash_table,nspp->name,nspp->surname,nspp->politicalparty,SIZE);
            pthread_mutex_unlock (&mut);
            if (flag==0){
                file= fopen(poll_log, "w");
                pthread_mutex_lock (&mut);
                flag=1;
                pthread_mutex_unlock (&mut);

            }
            fprintf(file , "Name: %s Surname: %s Party: %s \n", nspp->name,nspp->surname,nspp->politicalparty);
            close(result);
           
        }
        if (returning_num!=-1){
            printf("#5 Writing in (enalaktiko):%d \n",result);
            making_sure_write_sends(result, str3, (size_t)strlen(str3));
            close(result);
        }
        
        if (nspp != NULL){
            free(nspp);
        }

        if (buffer != NULL){
            free(buffer);
        }
        
        if (buffer2 != NULL){
            free(buffer2);
        }
        
    }
    
    pthread_exit(0);
}
////////////////////////////////////////////////////////////////////////////////////////////


int main(int argc , char * argv []){
    
    static struct sigaction handler ;
    handler.sa_handler = Handling_the_si ;
    sigfillset (&(handler.sa_mask));
    sigaction (SIGINT,&handler,NULL);

    ///////////////////////////////ARGV//////////////////////
    int portnum=atoi(argv[1]);
    int numWorkerthreads=atoi(argv[2]);
    int bufferSize=atoi(argv[3]);
    strcpy(poll_log,argv[4]);
    strcpy(poll_stats,argv[5]);
    /////////////////////////////////////////////////////////

    ///////////KEEPING THE SIZE OF THE BUFFER TO THE VARIABLE 'size'AND THE INITIALIZATION OF HASH TABLES////////////////////////////////////////
    size=bufferSize;  
    Hash_table = malloc(SIZE * sizeof(Hash_table_node));
    Initialization(Hash_table,SIZE);
    
    Hash_table_for_parties = malloc(16 * sizeof(Hash_table_party_node));
    Initialization_for_parties(Hash_table_for_parties,16);
    Inserting_for_parties(Hash_table_for_parties,16);
    //PrintHashTable( Hash_table,16);
    /////////////////////////////////////////////////////////

    /////////////////////////////////Initialization////////////////////
    pthread_mutex_init(&mut,NULL); 
    pthread_cond_init(&con_v_not_empty,NULL);
    pthread_cond_init(&con_v_not_full,NULL);
    //////////////////////////////////////////////////////


    ////////////// Initialization of the buffer //////////////////////////
    buff.fds=(int*)malloc(bufferSize * sizeof(int));//ARGV!!!!!!
    First_buff(&buff);
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
    pthread_t * Threadptr;
    Thread= malloc( numWorkerthreads * sizeof ( pthread_t ) );
    for ( int iter =0 ; iter < numWorkerthreads ; iter ++) {//Creating threads .
        Threadptr=Thread + iter;
        pthread_create( Threadptr , NULL , Purchaser , (void*)&buff ); 

    }
    ///////////////////////////////////////////////////////////////////


    while(signal_flag!=1){
    printf("About to accept a connection\n");
    ///////////////////////accept////////////////////////////////////////////
    int connect=accepting(socket_fd);
    if (connect<0&&signal_flag==1){
        printf("Accepting stops because of a interrupt signal \n");
        break;
    }

    if (connect<0&&signal_flag==0){
        printf("Error in accepting \n");
        break;
    }

    ////////////////////////////////////////////////////////////////////////////    


    ///////////////////////////////PRODUCER////////////////////////////////////////////
    printf("I just accepted a connection ->%d\n",connect);
    Provider(&buff,connect);
    
    ///////////////////////////////////////////////////////////////////////////////////////////////

    }

    pthread_cond_broadcast(&con_v_not_empty);
    fclose(file);

    ///////////////////////////////////////////Poll Stats file///////////////////////////////////////
    int the_final_count=0;
    FILE *file2 = fopen(poll_stats, "w");
    for (int i = 0; i < 16; i++) {
        if (Hash_table_for_parties[i].party[0] != '\0'&& Hash_table_for_parties[i].votes>0 ) {
            fprintf(file , "Party: %s  Votes: %d\n", Hash_table_for_parties[i].party,Hash_table_for_parties[i].votes);
            the_final_count=the_final_count+Hash_table_for_parties[i].votes ;
        }
    }

    fprintf(file,"Total number of votes:%d\n",the_final_count);
    fclose(file2);

    /////////////////////////////////////////////////////////////////////////////////////////////////

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

    close(socket_fd);

    ////////////Free memory//////////////////////
    free(Thread);
    free(buff.fds);
    /////////////////////////////////////////
    

    return 0;
}
