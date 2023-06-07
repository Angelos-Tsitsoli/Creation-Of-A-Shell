#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include "poller_interface.h"

typedef struct {
int key;
char  name[30];
char  surname[30];
char party[30];
}Hash_table_node;


void Initialization( Hash_table_node Hash_table[],int size)
{
for (int i=0; i<size; i++){
  Hash_table[i].key=-1;
  strcpy(Hash_table[i].name,"");
  strcpy(Hash_table[i].surname,"");
  }
return;
}


int Hashing(int key, int rows)
{
  return (key % rows);
}


int Double_hashing_increment(int key, int size )
{
  int temp=3-Hashing(key,size)%3;
  return temp;
}


void Insert(Hash_table_node Hash_table[],char * Name , char *Surname, char *party ,int size ){
int key = 0;
char storing_string[15];
strcpy(storing_string,Name);
for (int i = 0; i < strlen(Name); i++)
{
    key = key + (int)storing_string[i];
}

int position_in_table = Hashing(key, size);

int i=position_in_table;
int iterator =Double_hashing_increment(key,size);

if(Hash_table[i].key != -1){
  while (Hash_table[i].key != -1){
    i=i-iterator;
    if (i<0)
        i=i+size;
}
}

Hash_table[i].key=key;
strcpy(Hash_table[i].name,Name);
strcpy(Hash_table[i].surname,Surname);
strcpy(Hash_table[i].party,party);

return ;
}


int Search(Hash_table_node Hash_table[],char* name ,int size)
{
  int pr;
  int step;
  int key = 0;
  char storing_string[15];
  strcpy(storing_string,name);
  for (int i = 0; i < strlen(name); i++)
  {
      key = key + (int)storing_string[i];
  }
  /*Initializations */
  int i=Hashing(key,size);
  step=Double_hashing_increment(key,size);
  pr=Hash_table[i].key;
  /* Search loop */
  while ((key!=pr) && (pr!=-1)){
    i-=step;
    if (i<0)
    i+=size;
    pr=Hash_table[i].key;
  }
  /* Determine success or failure */
  if (pr==-1)
    return -1;
  else
    return i;
}

////////////////////////////////////////////////////////////////////////// inside thread ///////////////////////
//void Worker_action( int the_socket , Hash_table_node * Hash_table,int size, char * a_case ,char ** parties_and_names) {
//char single_char[1];
//char array [70];
//int i=0;
//int j=0;
//printf("MPHKA\n");
//while (read( the_socket , single_char , 1) > 0) { //PROSOXH APLA OTAN GRAFEIS ONOMA THA PRPEEI NA MHN BAZEIS SPACE STO TELOS AYTA
//    if(single_char[0]=='\n'){
//        if(strcmp(a_case,"party")==0){
//          parties_and_names[j] = (char *)malloc(60 * sizeof(char));
//          strcpy(parties_and_names[j],array);
//          strcpy(Hash_table[Search( Hash_table,name ,size)].party,parties_and_names[j]);
//          return;
//        }
//        
//    }
//    if((single_char[0]==' ')||(single_char[0]=='\n'&&strcmp(a_case,"name")==0)){
//        parties_and_names[j] = (char *)malloc(60 * sizeof(char));
//        strcpy(parties_and_names[j],array);
//        i=0;
//        j++;
//        Reseting(array);
//        if (single_char[0]=='\n'){
//          break;
//        }
//    }
//    
//    array[i]=single_char[0];
//    i++;
//}
//Insert(Hash_table, parties_and_names[0],parties_and_names[1] ,size);
//
//
//
////close (the_socket) ; /* Close socket */
////return 0;
//}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static int j=0;
//void Worker_action( int the_socket , Hash_table_node * Hash_table,int size, char * a_case ,char ** parties_and_names) {
//char single_char[1];
//char array [70];
//int i=0;
////int j=0;
////rintf("MPHKA\n");
//while (read( the_socket , single_char , 1) > 0) { //PROSOXH APLA OTAN GRAFEIS ONOMA THA PRPEEI NA MHN BAZEIS SPACE STO TELOS AYTA
//  if(strcmp(a_case,"name")==0){
//    if(single_char[0]==' '){
//      parties_and_names[j] = (char *)malloc(60 * sizeof(char));
//      strcpy(parties_and_names[j],array);
//      i=0;
//      j++;
//      Reseting(array);
//    }
//    if(single_char[0]=='\n'){
//      parties_and_names[j] = (char *)malloc(60 * sizeof(char));
//      strcpy(parties_and_names[j],array);
//      break;
//    }
//  }
//
//  if(strcmp(a_case,"vote")==0){
//    parties_and_names[j] = (char *)malloc(60 * sizeof(char));
//    strcpy(parties_and_names[j],array);
//    Insert(Hash_table, parties_and_names[0],parties_and_names[1],parties_and_names[2] ,size);
//    break;
//  }
//}
////Insert(Hash_table, parties_and_names[0],parties_and_names[1] ,size);
//
//
//
////close (the_socket) ; /* Close socket */
////return 0;
//}

