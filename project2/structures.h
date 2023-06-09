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


void Initialization_for_parties( Hash_table_node Hash_table[],char * Name , char *Surname, char *party ,int size)
{
  Inserting_to_hash( Hash_table,Name , char *Surname, char *party ,int size ) 
  

}

void Initialization( Hash_table_node Hash_table[],int size)
{
  for (int i=0; i<size; i++){
    Hash_table[i].key=-1;
    strcpy(Hash_table[i].name,"");
    strcpy(Hash_table[i].surname,"");
    strcpy(Hash_table[i].party,"");
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



void PrintHashTable(const Hash_table_node* Hash_table, int size) {
    for (int i = 0; i < size; i++) {
        if (Hash_table[i].name[0] != '\0' || Hash_table[i].surname[0] != '\0' || Hash_table[i].party[0] != '\0') {
            printf("Hash_table[%d]:\n", i);
            printf("Key: %d\n", Hash_table[i].key);
            printf("Name: %s\n", Hash_table[i].name);
            printf("Surname: %s\n", Hash_table[i].surname);
            printf("Party: %s\n", Hash_table[i].party);
            printf("\n");
        }
    }
}



void Inserting_to_hash(Hash_table_node Hash_table[],char * Name , char *Surname, char *party ,int size ){
  int key = 0;
  char storing_string[28];
  strcpy(storing_string,Name);
  strcat(storing_string,Surname);
  int length=strlen(Name)+strlen(Surname);
  for (int i = 0; i < length; i++)
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


int Search_in_hash(Hash_table_node Hash_table[],char* name ,char *surname,int size)
{
  int pr;
  int step;
  int key = 0;
  char storing_string[28];
  //printf("Here\n");
  strcpy(storing_string,name);
  strcat(storing_string,surname);
  int length=strlen(name)+strlen(surname);
  for (int i = 0; i < length; i++)
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

