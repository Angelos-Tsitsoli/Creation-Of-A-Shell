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

FILE *file_open;
char column1[70];
int iterator1=0;
int iterator1_final=0;
char column2[70];
int iterator2=0;
int iterator2_final=0;
char column3[70];
int iterator3=0;
int iterator3_final=0;
int column_iterate=0;
char letter;
char store[256];
int column_flag=1;
char* buffer=NULL;
char* buffer2=NULL;
char* buffer3=NULL;
int the_port;
char the_host[40];


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
    printf("I just wrote:%s\n",buffer);
}



void making_sure_read(int socket, char* buffer) {

    printf("In read\n");
    size_t messageSize2;

    printf("Reading size of bytes in :%d \n",socket);
    size_t bytesRead1 = read(socket, &messageSize2, sizeof(messageSize2));

    //printf("After the read and inside the making sure\n");
    
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
    printf("I just read:%s\n",buffer);
}



void * func(void * ptr){
    int socket_fd = *((int*)ptr);

    printf("Getting the host by name\n");
    struct hostent * Host = gethostbyname (the_host);


    struct sockaddr_in address_of_server;
   // struct sockaddr * serverptr = ( struct sockaddr *) & address_of_server ;

    address_of_server.sin_family = AF_INET;
    memcpy (&address_of_server.sin_addr , Host->h_addr_list[0], Host->h_length ) ;
    address_of_server.sin_port = htons(the_port);

    connect(socket_fd , (struct sockaddr*)&address_of_server , sizeof ( address_of_server ));
    //connect (socket_fd , serverptr , sizeof ( address_of_server ));
   

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

        
        printf("#1 Reading in :%d \n",socket_fd);
        making_sure_read(socket_fd,buffer);//1
        //printf("#2 Writing in :%d this:%s\n",socket_fd,buffer);
        making_sure_write_sends(socket_fd, store, (size_t)strlen(store));//2
        printf("#3 Reading in :%d \n",socket_fd);
        making_sure_read(socket_fd,buffer2);//3
        //printf("%s\n",buffer2);

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
            printf("The read #5\n");
            

        }

        free(buffer);
        free(buffer2);
        free(buffer3);
        buffer = malloc(17);
        buffer2 = malloc(17);
        buffer3 = malloc(30); 
        Reseting(column1);
        Reseting(column2);
        Reseting(column3);
        Reseting(store);
        iterator1_final=0;
        iterator2_final=0;
        iterator3_final=0;
        iterator=0;
        close(socket_fd);
        printf("Telos thread\n");
        pthread_exit((void*)0);
}




int main(int argc , char * argv []){


    printf("Opening the file\n");
    file_open = fopen(argv[3], "r");

    strcpy(the_host,argv[1]);
    the_port=atoi(argv[2]);
    int threadExitStatus;

    buffer = malloc(17);
    buffer2 = malloc(17);      
    buffer3 = malloc(30);  

    if (buffer == NULL || buffer2 == NULL || buffer3 == NULL) {
        perror("malloc");
        return -1;
    }


    while (1) {

        letter= fgetc(file_open); 

        if (feof(file_open)) {
            printf("I am about to leave\n");
            free(buffer);
            free(buffer2);
            free(buffer3);
            fclose(file_open); 
            break;
        }

        if(column_flag==3&&letter=='\n'){

            int socket_fd = creating_socket();
            if (socket_fd < 0) {
                printf("Error creating socket\n");
                return -1;
            }

            column_flag=1;
            iterator3_final=iterator3;
            iterator3=0;

            pthread_t  Thread;
            int result=pthread_create( &Thread , NULL , func , &socket_fd );
            if (result != 0) {
                printf("ERROR IN CREATE\n");
            }
            printf("Telos func\n");
            result=pthread_join(Thread,(void**)&threadExitStatus);
            if (result != 0) {
                printf("ERROR IN JOIN\n");
            }
            
            if (threadExitStatus == 0) {
        // pthread_exit was called successfully
                printf("pthread_exit was called successfully.\n");
            } else {
        // pthread_exit failed or returned a non-zero exit status
                printf("pthread_exit failed or returned a non-zero exit status.\n");
            }

            printf("END\n");
        }

         if(column_flag==2&&letter==' '){
            column_flag=3;
            iterator2_final=iterator2;
            iterator2=0;
        }



        if(column_flag==1&&letter==' '){
            column_flag=2;
            iterator1_final=iterator1;
            iterator1=0;
        }



        if(column_flag==1&&letter!='\n'){
            column1[iterator1]=letter;
            iterator1++;
        }

        if(column_flag==2&&letter!='\n'){
            if(letter!=' '){
                column2[iterator2]=letter;
                iterator2++;
            }
        }

        if(column_flag==3&&letter!='\n'){
            if(letter!=' '){
                column3[iterator3]=letter;
                iterator3++;
            }
        }


        printf("Also end\n");
    }

    printf("I am about to end\n");
    //fclose(file_open);
    sleep(2);
    if (fclose(file_open) == EOF) {
        perror("fclose");
    }
    return 0;
}