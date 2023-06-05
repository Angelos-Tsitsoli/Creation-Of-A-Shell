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
int portNum=8;//atoi(argv[2]);
int bufferSize=16;//atoi(argv[3]);
FILE *file_open;
//char line[70];
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
file_open = fopen("inputFile.txt", "r");


int socket_fd=creating_socket();
if (socket_fd<0){
    printf("Error creating socket\n");
    return -1;
}

struct hostent * Host = gethostbyname ( "linux04.di.uoa.gr"/*argv [1]*/);


int the_port=5634;/*atoi ( argv [2])*/


struct sockaddr_in address_of_server;
address_of_server.sin_family = AF_INET;
//address_of_server.sin_addr.s_addr = htonl(INADDR_ANY);
memcpy (&address_of_server.sin_addr , Host->h_addr_list , Host->h_length ) ;
address_of_server.sin_port = htons(the_port);


connect(socket_fd , (struct sockaddr*)&address_of_server , sizeof ( address_of_server ));


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
            store[i]=column1;

        }

        store[iterator1_final]=' ';

        for(int i=0; i<iterator2_final;i++){
            store[i]=column2;

        }

        store[iterator1_final]='\0';
        size_t length = strlen(store);

        write (socket_fd , store , length);

        while (read( the_socket , single_char , 1) > 0){


        }

        if () {

            write ...
        }

        else () {
            continue;
        }


        Reseting(column1);
        Reseting(column2);
        Reseting(column3);
        iterator1_final=0;
        iterator2_final=0;
        iterator3_final=0;
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
        column2[iterator2]=letter;
        iterator2++;
    }

    if(column_flag==3&&letter!='\n'){
        column3[iterator3]=letter;
        iterator3++;
    }



}

fclose(file_open);
return 0;








}