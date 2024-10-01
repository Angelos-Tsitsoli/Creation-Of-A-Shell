#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "interface.h"
#include <string.h>

static int position=1;
static int number_of_alias=1;


///////////////////////////////////////////////////////////////////    The following functions are made for the History of the shell structure
void Initialize_history(struct History **H)
{
   *H = malloc(sizeof *H);
   (*H)->Front = NULL;
   (*H)->Rear = NULL;
}

void Insertion_to_history(struct History *H, char *command)
{  
   Node_ptr temp = malloc(sizeof *temp);
   strcpy(temp->Command, command);
   if (H->Number_of_commands % 20==0){
      position=1;
   }
   temp->position=position;
   position++;
   
   if (H->Front == NULL)
   {
      
      H->Front = temp;
      H->Front->Nextcommand = NULL;
      H->Rear = H->Front;
      H->Number_of_commands++;
      
   }
   else
   {
      if (H->Number_of_commands == 20||H->Number_of_commands>20)
      {
         
         H->Number_of_commands++;
         Node_ptr iterator;
         iterator = H->Front;
         iterator=iterator->Nextcommand;
         free(H->Front);
         H->Front=iterator;
         H->Rear->Nextcommand=temp;
         H->Rear=temp;
         temp->Nextcommand = NULL;
         
      }

      if (H->Number_of_commands < 20) 
      {  
         H->Number_of_commands++;
         H->Rear->Nextcommand=temp;
         H->Rear=temp;
         temp->Nextcommand = NULL;
      }
   }
}


char * Get_history_command(struct History *H,int hist_position){
Node_ptr iterator;
iterator=H->Front;
while(iterator!=NULL){

   if(iterator->position==hist_position){
         return iterator->Command;

   }
   iterator=iterator->Nextcommand;

}

printf("Sorry i couldnt find it\n");

return NULL;

}



void Print_history(struct History *H)
{

   Node_ptr temp;
   temp = H->Front;
   while (temp != NULL)
   {
      printf("%d. %s\n",temp->position, temp->Command );
      temp = temp->Nextcommand;
   }
}


void Free_history(struct History * H){
Node_ptr temp=H->Front;
Node_ptr temp2;
if (H->Front!=NULL){
   do{
      temp2=temp;
      temp=temp->Nextcommand;
      free(temp2);

   } while(temp!=NULL);
   }

else if (H->Front==NULL){
   free(H);
}

}



////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////// The following functions are made for the Alias structure.
void Initialize_alias_structure(struct Alias **A)
{
   *A = malloc(sizeof *A);
   (*A)->Front = NULL;
   (*A)->Rear = NULL;
}


void Insertion_to_alias_structure(struct Alias *A, char *command1,char * command2){
Alias_node_ptr temp = malloc(sizeof *temp);
strcpy(temp->alias_command, command1);
strcpy(temp->original_command, command2);
temp->position=number_of_alias;
number_of_alias++;
if (A->Front == NULL)
   {
      A->Front = temp;
      A->Front->Nextcommand = NULL;
      A->Rear = A->Front;
      //A->Number_of_commands++;
   }
else if (A->Front != NULL){
      temp->Nextcommand=A->Front;
      A->Front=temp;

}
return;
}


char * Check_alias_command(struct Alias *A,char * command){
Alias_node_ptr iterator;
iterator=A->Front;
while(iterator!=NULL){

   if(strcmp(command,iterator->alias_command)==0){
         return iterator->original_command;

   }
   iterator=iterator->Nextcommand;

}

//printf("Sorry i couldnt find it\n");

return NULL;

}



void Destroy_an_alias(struct Alias *A,char * command){
Alias_node_ptr iterator;
Alias_node_ptr iterator2;
Alias_node_ptr iterator3;
iterator=A->Front;
iterator2=A->Front;

int temp;

if(iterator==NULL){
   printf("No aliases were stored\n");
   return;


}
if(iterator->Nextcommand==NULL){
   free(iterator);
   number_of_alias--;
   A->Front=NULL;
   A->Rear=NULL;
   return;

}

if(strcmp(command,iterator->alias_command)==0){
   A->Front=A->Front->Nextcommand;
   free(iterator);
   number_of_alias=number_of_alias-2;
   temp=number_of_alias;
   iterator3=A->Front;
   while(iterator3!=NULL){
      iterator3->position=temp;
      iterator3=iterator3->Nextcommand;
      temp--;

   }
   number_of_alias++;
   return;
}


while(iterator!=NULL){

   if(strcmp(command,iterator->alias_command)==0){
         if(iterator2->Nextcommand!=iterator){
            while(iterator2->Nextcommand!=iterator){
               iterator2=iterator2->Nextcommand;
            }
         }
         if (iterator->Nextcommand!=NULL){
            iterator2->Nextcommand=iterator->Nextcommand;
         }
         A->Front=A->Front->Nextcommand;
         free(iterator);
   }


   iterator=iterator->Nextcommand;

}
number_of_alias=number_of_alias-2;
temp=number_of_alias;
while(iterator3!=NULL){
   iterator3->position=temp;
   iterator3=iterator3->Nextcommand;
   temp--;

}
number_of_alias++;


return;

}




void Print_alias(struct Alias *A)
{

   Alias_node_ptr temp;
   temp = A->Front;
   if(A->Front==NULL){
      printf("There are no aliases stored\n");
   }
   while (temp != NULL)
   {
      printf("%d. alias:%s | original:%s\n",temp->position , temp->alias_command,temp->original_command);
      temp = temp->Nextcommand;
   }
}


void Free_alias(struct Alias * A){
Alias_node_ptr temp=A->Front;
Alias_node_ptr temp2;
if (A->Front!=NULL){
   do{
      temp2=temp;
      temp=temp->Nextcommand;
      free(temp2);

   } while(temp!=NULL);
   }

else if (A->Front==NULL){
   free(A);
}

}

/////////////////////////////////////////////////////////////////////////////////////////////////////