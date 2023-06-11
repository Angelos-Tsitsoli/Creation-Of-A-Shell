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


typedef struct {
  int key;
  char party[30];
  int votes;
}Hash_table_party_node;


int Hashing(int key, int rows)
{
  int final=key % rows;
  return final;
}


int Double_hashing_increment(int key, int size )
{
  int an_odd_number=3;
  int hash_num=Hashing(key,size);
return an_odd_number-hash_num%an_odd_number;
}



void Inserting_party_to_hash(Hash_table_party_node Hash_table[], char *party, int size)  {
    int the_final_key = 0;
    char storing_string[30];
    strcpy(storing_string, party);
    int length ;
    length=strlen(party);

    for (int i = 0; i < length; i++) {
        the_final_key = the_final_key + (int)storing_string[i];
    }

    int position_in_table = Hashing(the_final_key, size);
    int iterator2 = position_in_table;
    int iterator = Double_hashing_increment(the_final_key, size);

    int check_val;
    check_val=Hash_table[iterator2].key;

    if ( check_val!= -1) {
        while (Hash_table[iterator2].key != -1) {
            iterator2 = iterator2 - iterator;
            if (iterator2 < 0)
                iterator2 = iterator2 + size;
        }
    }

    Hash_table[iterator2].key = the_final_key;
    strcpy(Hash_table[iterator2].party, party);
}


void Inserting_for_parties( Hash_table_party_node Hash_table[],int size)
{
  
  Inserting_party_to_hash( Hash_table,"NEA_DIMOKRATIA", size );
  Inserting_party_to_hash( Hash_table,"SYRIZA", size );
  Inserting_party_to_hash( Hash_table,"KKE", size );
  Inserting_party_to_hash( Hash_table,"PASOK", size ); 
  Inserting_party_to_hash( Hash_table,"NIKH", size );
  Inserting_party_to_hash( Hash_table,"MERA25", size );  
  Inserting_party_to_hash( Hash_table,"ELLHNIKH_LYSH", size );
  Inserting_party_to_hash( Hash_table,"KOTES", size ); 
  Inserting_party_to_hash( Hash_table,"ANTARSYA", size );
  Inserting_party_to_hash( Hash_table,"EAN", size );
  Inserting_party_to_hash( Hash_table,"SOCIAL", size );
  Inserting_party_to_hash( Hash_table,"PRASINO_KINHMA", size );
  Inserting_party_to_hash( Hash_table,"ELLHNVN_SYNELEFSIS", size );
  Inserting_party_to_hash( Hash_table,"PLEFSI_ELEFTHERIAS", size );
  
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

void Initialization_for_parties(Hash_table_party_node Hash_table[],int size)
{
  for (int i=0; i<size; i++){
    Hash_table[i].key=-1;
    Hash_table[i].votes=0;
    strcpy(Hash_table[i].party,"");
    }
  return;
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


void PrintHashTableForParties(const Hash_table_party_node* Hash_table, int size) {
    for (int i = 0; i < size; i++) {
        if (Hash_table[i].party[0] != '\0') {
            printf("Hash_table[%d]:\n", i);
            printf("Key: %d\n", Hash_table[i].key);
            printf("Votes: %d\n", Hash_table[i].votes);
            printf("Party: %s\n", Hash_table[i].party);
            printf("\n");
        }
    }
}






void Inserting_to_hash(Hash_table_node Hash_table[],char * Name , char *Surname, char *party ,int size ){
  int  the_final_key  = 0;
  char storing_string[28];
  strcpy(storing_string,Name);
  strcat(storing_string,Surname);
  int length;
  length =strlen(Name)+strlen(Surname);
  for (int i = 0; i < length; i++)
  {
      the_final_key = the_final_key + (int)storing_string[i];
  }

  int position_in_table = Hashing(the_final_key, size);

  int iterator2 =position_in_table;
  int iterator =Double_hashing_increment(the_final_key,size);

  int check_val;
  check_val=Hash_table[iterator2].key;

  if(check_val != -1){
    while (Hash_table[iterator2 ].key != -1){
      iterator2 =iterator2 -iterator;
      if (iterator2 <0)
          iterator2 =iterator2 +size;
  }
  }

  Hash_table[iterator2 ].key=the_final_key;
  strcpy(Hash_table[iterator2 ].name,Name);
  strcpy(Hash_table[iterator2 ].surname,Surname);
  strcpy(Hash_table[iterator2 ].party,party);

  return ;
}


int Search_in_hash(Hash_table_node Hash_table[],char* name ,char *surname,int size)
{
  int pr=0;
  int step=0;
  int key = 0;
  char storing_string[28];
  strcpy(storing_string,name);
  strcat(storing_string,surname);
  int length;
  length=strlen(name)+strlen(surname);
  for (int i = 0; i < length; i++)
  {
      key = key + (int)storing_string[i];
  }
  int final_return=Hashing(key,size);
  step=Double_hashing_increment(key,size);
  pr=Hash_table[final_return].key;
  while (key!=pr && pr!=-1){
    final_return=final_return-step;
    if (final_return<0){
      final_return=final_return + size;
    }
    pr=Hash_table[final_return].key;
  }

  if (pr==-1){
    return -1;}

  return final_return;
}


int Search_in_hash_party(Hash_table_party_node Hash_table[],char* party ,int size)
{
  int pr=0;
  int step=0;
  int key = 0;
  char storing_string[28];
  
  strcpy(storing_string,party);
  for (int i = 0; i < strlen(party); i++)
  {
      key = key + (int)storing_string[i];
  }
  
  int final_return=Hashing(key,size);
  step=Double_hashing_increment(key,size);
  pr=Hash_table[final_return].key;
  
  while (key!=pr && pr!=-1){
    
    final_return=final_return-step;
    if (final_return<0){
    final_return=final_return+size;}
    pr=Hash_table[final_return].key;
  }
  
  if (pr==-1)
    return -1;
  
  return final_return;
}








