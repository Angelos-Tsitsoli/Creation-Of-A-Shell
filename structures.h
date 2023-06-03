#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <ctype.h>
#include <string.h>


typedef struct {
int key;
char  name[30];
char  surname[30];
}Hash_table_node;


void Initialization(Hash_table_node Hash_table[],int size)
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



void Insert(Hash_table_node Hash_table[],char * Name , char *Surname, int size ){
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

return ;
}






