#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <netdb.h> 
#include <stdlib.h> 
#include <string.h> 
#include "poller_interface.h"
#include <pthread.h>

FILE *file_open; //for input file
char column1[70];// name of  voter
int iterator1=0;
int iterator1_final=0;
char column2[70]; //surname of voter 
int iterator2=0;
int iterator2_final=0;
char column3[70]; // political party
int iterator3=0;
int iterator3_final=0;
int column_iterate=0;
char letter; // letter iterator of file
char store[256]; // through this the messages will be sent from the client
int column_flag=1;  // to know everytime what column of a row we read in the file inputfile
char* buffer=NULL;  // through these buffers we will store the messages we get
char* buffer2=NULL;
char* buffer3=NULL;
int the_port;    
char the_host[40];  



//////////////////////////////////Write the size of the message and the message////////////////////
void making_sure_write_sends(int socket, char* buffer, size_t bufferSize) {  
    size_t bytessent = 0;
    size_t byte;
    
    size_t headerSize = sizeof(size_t);
    size_t bufferSizeNetwork = htonl(bufferSize);
    printf("Writing size of bytes in :%d \n",socket);
    size_t bytesSent = write(socket, &bufferSizeNetwork, headerSize);
    
    printf("Writing the data in :%d the %s\n",socket,store);
    while (bytessent < bufferSize) {
        byte = write(socket, buffer + bytessent, bufferSize - bytessent);

        if (byte == -1) {
            perror("write");
            break;
        }

        bytessent += byte;
    }
    printf("End of writing %zu bytes\n", bytessent);
}
///////////////////////////////////////////////////////////////////////////////////////////

///////////////Reading the size of the message and the message from the server///////////////////////
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
    while (bytesReceived < messageSize2) {
        bytesRead = read(socket, buffer + bytesReceived, messageSize2 - bytesReceived);

        if (bytesRead == -1) {
            perror("read");
            break;
        } else if (bytesRead == 0) {
            // Connection closed by the server
            printf("bytesRead are 0\n");
            break;
        }

        bytesReceived += bytesRead;
    }

    printf("End of reading: %ld bytes received\n", bytesReceived);

}
/////////////////////////////////////////////////////////////////////////////////////


/////////////////////////This function is going to be called when the thread is created in order to do the writing and the reading of messages//////
void * func(void * ptr){
    int socket_fd = *((int*)ptr);

    printf("Getting the host by name\n");
    struct hostent * Host = gethostbyname (the_host);


    struct sockaddr_in address_of_server;

    address_of_server.sin_family = AF_INET;
    memcpy (&address_of_server.sin_addr , Host->h_addr_list[0], Host->h_length ) ;
    address_of_server.sin_port = htons(the_port);

    connect(socket_fd , (struct sockaddr*)&address_of_server , sizeof ( address_of_server ));

   
    //////////////////////giving to store the name and the surname///////////////////////////
    for(int i=0; i<iterator1_final;i++){
            store[i]=column1[i];

        }

        store[iterator1_final]=' ';
        

        int j=0;
        for(int i=iterator1_final+1; j<iterator2_final;i++,j++){
            
            store[i]=column2[j];

        }

        j=0;
        store[iterator1_final+iterator2_final+1]='\0';
        size_t length = strlen(store);
    /////////////////////////////////////////////////////////


        //////////////////////communication with the server///////////////////////////    
        printf("#1 Reading in :%d \n",socket_fd);
        making_sure_read(socket_fd,buffer);//1
        making_sure_write_sends(socket_fd, store, (size_t)strlen(store));//2
        printf("#3 Reading in :%d \n",socket_fd);
        making_sure_read(socket_fd,buffer2);//3

        char one_char[1];
        int iterator=0;


        if (strcmp(buffer2,"SEND VOTE PLEASE")==0) {
            Reseting(store);
            length = strlen(store);
            for(int i=0; i<iterator3_final;i++){
                store[i]=column3[i];

            }
           
            printf("#4 Writing in :%d \n",socket_fd);
            making_sure_write_sends(socket_fd, store, (size_t)strlen(store));//4
            printf("#5 Reading in :%d \n",socket_fd);
            making_sure_read(socket_fd,buffer3);//5
            printf("buffer3-> %s\n",buffer3);

            

        }
        /////////////////////////////////////////////////////////////////////


        //////////////////free the space from buffers in order to delete everything inside and allocating space again for the next use
        free(buffer);
        free(buffer2);
        free(buffer3);
        buffer = malloc(17);
        buffer2 = malloc(17);
        buffer3 = malloc(30); 
        ///////////////////////reseting the arrays (deleting what they had inside)/////////////////////////
        Reseting(column1);
        Reseting(column2);
        Reseting(column3);
        Reseting(store);
        //////////////////////////// initializing the iterator again with zero ////////////
        iterator1_final=0;
        iterator2_final=0;
        iterator3_final=0;
        iterator=0;
        /////////////closing and exiting
        close(socket_fd);
        printf("End of thread\n");
        pthread_exit((void*)0);
}
/////////////////////////////////////////////////////////////////////////////////////////////



int main(int argc , char * argv []){

    /////////opening/////////////////////////
    printf("Opening the file\n");
    file_open = fopen(argv[3], "r");
    ///////////////////////////////////
    //////// storing the port that the communication will happen/////////
    strcpy(the_host,argv[1]);
    the_port=atoi(argv[2]);
    /////////////////////////////////////////////////////////\

    int threadExitStatus;

    buffer = malloc(17);
    buffer2 = malloc(17);      
    buffer3 = malloc(30);  
    ///////////////////////check if the malloc was successful//////////
    if (buffer == NULL || buffer2 == NULL || buffer3 == NULL) {
        perror("malloc");
        return -1;
    }
    ///////////////////////////////////////////////////////////

    /////////////////////////////Do the following untill the the whole inputfile is read/////////////////////////////
    while (1) {
        /////////////reading every character///////////////
        letter= fgetc(file_open); 
        ////////////////////////////////////////////////

        ///////////////////////When the whole file is read///////////////////
        if (feof(file_open)) {
            printf("I am about to exit\n");
            free(buffer);
            free(buffer2);
            free(buffer3);
            
            int file_descriptor = fileno(file_open);
            close(file_descriptor); 
            
            break;
        }
        ///////////////////////////////////////////////////////////////

        ///////////////////When we read the symbol "\n" in the file//////////////////////////////
        if(column_flag==3&&letter=='\n'){

            ////////////create socket/////////////
            int socket_fd = creating_socket();
            if (socket_fd < 0) {
                printf("Error creating socket\n");
                return -1;
            }
            ////////////////////////////////

            ////////////set the flag to 1 in order to know which column we read after these actions the follow
            column_flag=1;
            //////////////////////////////////////
            //////what number it stopped when iterating the third column////////////////////
            iterator3_final=iterator3;
            //////////resetting the main iterator of this column 
            iterator3=0;

            //////////////////creating the thread//////////
            pthread_t  Thread;
            int result=pthread_create( &Thread , NULL , func , &socket_fd );
            if (result != 0) {
                printf("ERROR IN CREATE\n");
            }
            /////////////////////////

            /////////////waiting the thread/////////////////////
            result=pthread_join(Thread,(void**)&threadExitStatus);
            if (result != 0) {
                printf("ERROR IN JOIN\n");
            }
            //////////////////////////////////


            ////////////////in order to know if it exited successfully
            if (threadExitStatus == 0) {
        // pthread_exit was called successfully
                printf("pthread_exit was called successfully.\n");
            } else {
        // pthread_exit failed or returned a non-zero exit status
                printf("pthread_exit failed or returned a non-zero exit status.\n");
            }
            //////////////////////////////
        }
        ////////////////////////////////////////////////////////////////////////////////////


        //////////////// If we find space symbol and we read second column/////////////////////
         if(column_flag==2&&letter==' '){
            column_flag=3;
            iterator2_final=iterator2;
            iterator2=0;
        }
        /////////////////////////////////////////////////////////


        ///////////////////////////If we find space and we read the first column/////////////
        if(column_flag==1&&letter==' '){
            column_flag=2;
            iterator1_final=iterator1;
            iterator1=0;
        }
        //////////////////////////////////////////////////


        ////////////////////Storing in the first column/////////////
        if(column_flag==1&&letter!='\n'){
            column1[iterator1]=letter;
            iterator1++;
        }

        ///////////////////Storing in the second column/////////////
        if(column_flag==2&&letter!='\n'){
            if(letter!=' '){
                column2[iterator2]=letter;
                iterator2++;
            }
        }

        ///////////////////Storing in the third column/////////////
        if(column_flag==3&&letter!='\n'){
            if(letter!=' '){
                column3[iterator3]=letter;
                iterator3++;
            }
        }


    
    }
/////////////////////////////////////////////////////////////////////////
    return 0;
}