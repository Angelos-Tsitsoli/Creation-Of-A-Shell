#include <stdio.h>
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <unistd.h> 
#include <netdb.h> 
#include <stdlib.h> 
#include <string.h> 
#include "poller_interface.h"

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

char buffer[40];
char reading;
int it=0;

printf("Connected to %d\n",socket_fd );
sleep(1);
//while (read( socket_fd , &reading , 1) > 0 && reading!='\n'){
//            printf("%c\n",reading);
//            buffer[it]=reading;
//            it++;
//        }
//
//printf("After read client\n");
//printf("%s\n",buffer);


char buf [1];
while ( read ( socket_fd , buf , 1) > 0) { /* Receive 1 char */
    //putchar ( buf [0]) ; /* Print received char */
    /* Capitalize character */
    buffer[it]=reading;
    it++;
}


printf("After read client\n");
printf("%s\n",buffer);


//
//while (!feof(file_open)) {
//
//    letter= fgetc(file_open); 
//
//    if(column_flag==3&&letter=='\n'){
//        column_flag=1;
//        iterator3_final=iterator3;
//        iterator3=0;
//        //printf("First column:\n");
//        //for (int i = 0; i<20; i++) {
//        //    printf("%c", column3[i]);
//        //}
//        //printf("\n");
//        for(int i=0; i<iterator1_final;i++){
//            store[i]=column1[i];
//
//        }
//
//        store[iterator1_final]=' ';
//
//        for(int i=iterator1_final; i<iterator1_final+iterator2_final;i++){
//            store[i]=column2[i];
//
//        }
//
//        store[iterator1_final+iterator2_final]='\0';
//        size_t length = strlen(store);
//
//        printf("First writing to the socket\n");
//        write (socket_fd , store , length);
//
//        char array[60];
//        char one_char[1];
//        int iterator=0;
//        while (read( socket_fd , one_char , 1) > 0){
//            array[iterator]=one_char[0];
//            iterator++;
//        }
//
//        array[iterator+1]='\0';
//
//        if (strcmp(array,"SEND VOTE PLEASE")==0) {
//            Reseting(store);
//            length = strlen(store);
//            for(int i=0; i<iterator3_final;i++){
//                store[i]=column3[i];
//
//            }
//            printf("Second writing to the socket\n");
//            write(socket_fd , store , length);
//        }
//
//        //if  (strcmp(array,"ALREADY VOTED")==0) {
//        //    continue;
//        //}
////
//
//        Reseting(column1);
//        Reseting(column2);
//        Reseting(column3);
//        Reseting(store);
//        Reseting(array);
//        iterator1_final=0;
//        iterator2_final=0;
//        iterator3_final=0;
//        iterator=0;
//    }
//
//     if(column_flag==2&&letter==' '){
//        column_flag=3;
//        iterator2_final=iterator2;
//        iterator2=0;
//    }
//
//
//
//    if(column_flag==1&&letter==' '){
//        column_flag=2;
//        iterator1_final=iterator1;
//        iterator1=0;
//    }
//
//
//
//    if(column_flag==1&&letter!='\n'){
//        column1[iterator1]=letter;
//        iterator1++;
//    }
//
//    if(column_flag==2&&letter!='\n'){
//        column2[iterator2]=letter;
//        iterator2++;
//    }
//
//    if(column_flag==3&&letter!='\n'){
//        column3[iterator3]=letter;
//        iterator3++;
//    }
//
//
//
//}

fclose(file_open);
return 0;








}