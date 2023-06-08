#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <netdb.h> 
#include <stdlib.h> 
#include <string.h> 
#include "poller_interface.h"


void making_sure_write_sends(int socket, char* buffer, size_t bufferSize) {
    printf("Writing\n");
    size_t bytessent = 0;
    size_t byte;
    printf("HERE\n");
    size_t headerSize = sizeof(size_t);
    size_t bufferSizeNetwork = htonl(bufferSize);
    size_t bytesSent = write(socket, &bufferSizeNetwork, headerSize);
    printf("FINISH\n");

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




void making_sure_read(int socket, char* buffer) {

    printf("Hello\n");
    size_t messageSize2;

    size_t bytesRead1 = read(socket, &messageSize2, sizeof(messageSize2));

    
    if (bytesRead1 != sizeof(messageSize2)) {
        perror("read");
        return;
    }

    messageSize2 = ntohl(messageSize2);  // Convert message size to host byte order

    printf("Message size: %zu\n", messageSize2);



    printf("Reading\n");
    size_t bytesReceived = 0;
    size_t bytesRead;

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




int main(/*int argc , char * argv []*/){

char serverName[40];//atoi(argv[1]);
int bufferSize=16;//atoi(argv[3]);
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
char buff[40];

printf("Opening the file\n");
file_open = fopen("inputFile.txt", "r");


int socket_fd=creating_socket();
if (socket_fd<0){
    printf("Error creating socket\n");
    return -1;
}

printf("Getting the host by name\n");
struct hostent * Host = gethostbyname ( "localhost"/*argv [1]*/);


int the_port=5634;/*atoi ( argv [2])*/

struct sockaddr_in address_of_server;
struct sockaddr * serverptr = ( struct sockaddr *) & address_of_server ;

address_of_server.sin_family = AF_INET;
//address_of_server.sin_addr.s_addr = htonl(INADDR_ANY);
//memcpy (& server . sin_addr , rem - > h_addr , rem - > h_length ) ;
memcpy (&address_of_server.sin_addr , Host->h_addr_list[0], Host->h_length ) ;
address_of_server.sin_port = htons(the_port);

printf("Connecting to a socket\n");
//connect(socket_fd , (struct sockaddr*)&address_of_server , sizeof ( address_of_server ));
connect (socket_fd , serverptr , sizeof ( address_of_server ));

char reading;
int it=0;

printf("Connected to %d\n",socket_fd );


printf("Reading\n");
 size_t bytesRead=16;
 size_t dataSize;
 char* buffer = malloc(18);
 char* buffer2 = malloc(18);
    if (buffer == NULL) {
        perror("malloc");
    }
 char* buffer3 = malloc(30);  
//making_sure_read(socket_fd,buffer);//1

//printf("After read client\n");
//printf("%s\n",buffer);


///////////////////////////////////////////////////////////////////
//char* buffer2 = malloc(120);
//char str[20] = "ONOMA EPITHETO";
//making_sure_write_sends(socket_fd, str, (size_t)strlen(str));//2
//printf("After write client\n");
//making_sure_read(socket_fd,buffer2);//3
//printf("%s\n",buffer2);
//char* buffer3 = malloc(20);
//char str5[20] = "PASOK";
//making_sure_write_sends(socket_fd, str5, (size_t)strlen(str));//4
//making_sure_read(socket_fd,buffer3);//5
//printf("%s\n",buffer3);
/////////////////////////////////////////////////////////////////////





while (!feof(file_open)) {

    letter= fgetc(file_open); 

    if(column_flag==3&&letter=='\n'){
        column_flag=1;
        iterator3_final=iterator3;
        iterator3=0;
        //printf("First column:\n");
        //for (int i = 0; i<20; i++) {
        //    printf("%c", column3[i]);
        //}
        //printf("\n");
        for(int i=0; i<iterator1_final;i++){
            store[i]=column1[i];

        }

        store[iterator1_final]=' ';
        //printf("oooooo %s %d\n",store,iterator1_final);

        int j=0;
        for(int i=iterator1_final+1; j<iterator2_final;i++,j++){
            //printf("E-> %c",column2[j]);
            store[i]=column2[j];

        }

        j=0;
        //printf("ppppp %c \n",store[iterator1_final+iterator2_final-1]);
        store[iterator1_final+iterator2_final+1]='\0';
        size_t length = strlen(store);

        //printf("First writing to the socket\n");
        //write (socket_fd , store , length);
        //sleep(1);
        //printf("eeeeeee1 %s\n",store);
        //printf("eeeeeee2 %s\n",column1);
        //printf("eeeeeee3 %s\n",column2);
        //printf("eeeeeee4 %s\n",column3);

        making_sure_read(socket_fd,buffer);//1
        making_sure_write_sends(socket_fd, store, (size_t)strlen(store));//2
        making_sure_read(socket_fd,buffer2);//3
        //printf("RRRR%s\n",buffer3);

        //char array[60];
        char one_char[1];
        int iterator=0;
       // while (read( socket_fd , one_char , 1) > 0){
       //     array[iterator]=one_char[0];
       //     iterator++;
       // }

        //array[iterator+1]='\0';

        if (strcmp(buffer2,"SEND VOTE PLEASE")==0) {
            Reseting(store);
            length = strlen(store);
            for(int i=0; i<iterator3_final;i++){
                store[i]=column3[i];

            }
            //printf("Second writing to the socket\n");
            //write(socket_fd , store , length);
            making_sure_write_sends(socket_fd, store, (size_t)strlen(store));
            making_sure_read(socket_fd,buffer3);
            printf("%s\n",buffer3);

        }

        //if  (strcmp(array,"ALREADY VOTED")==0) {
        //    continue;
        //}
//

        Reseting(column1);
        Reseting(column2);
        Reseting(column3);
        Reseting(store);
        //Reseting(array);
        iterator1_final=0;
        iterator2_final=0;
        iterator3_final=0;
        iterator=0;
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



}

fclose(file_open);
return 0;








}