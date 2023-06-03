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


Hash_table_node *Hash_table;
int size=10;

void Initialization(Hash_table_node Hash_table[])
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


int Double_hashing_increment(int key)
{
  int temp=3-Hashing(key,size)%3;
  return temp;
}

void Insert(Hash_table_node Hash_table[],char * Name , char *Surname){
int key = 0;
char storing_string[15];
strcpy(storing_string,Name);
for (int i = 0; i < strlen(Name); i++)
{
    key = key + (int)storing_string[i];
}

int position_in_table = Hashing(key, size);

int i=position_in_table;
int iterator =Double_hashing_increment(key);

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

int main(){
char  name[20];
char surname[20];
Hash_table = malloc(30 * sizeof(Hash_table_node));
Initialization(Hash_table);
for(int i=0;i<30;i++){
  printf("%d.->%d\n",i,Hash_table[i].key);
}

for(int i=0;i<size;i++){
  scanf("%s", name);
  //scanf("%s", surname);
  Insert( Hash_table,name,"urname" );
}


for(int i=0;i<30;i++){
  printf("%d.->%s , %s\n",i,Hash_table[i].name,Hash_table[i].surname);
}


free(Hash_table);
}






