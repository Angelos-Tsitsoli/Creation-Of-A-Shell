#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "interface.h"
#include <string.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>

int main() {

struct History * history; //This will be the linked list for the storage of the commands in the history 
Initialize_history(&history); //Initializing the history 
struct Alias * an_alias; //This will be the structure for the aliases
Initialize_alias_structure(&an_alias); //Initializing the alias structure
enum argument result; //This takes the result that will be returned inside the third while and decides if to continue or not
signal_avoidance();  //This function at this points ignores signals

while(1){
    char input[400];  //This will be the input we get from fgets.
    char * iterator;  //I use this pointer in order to get rid of the \n symbol later from the command when i give it . 
    iterator=input;
    printf("in-mysh-now:>");  //The prompt.
    fgets(input, sizeof(input), stdin);  //To get the input.
    Clean(); //This will bring back the last position every time
    ///////////////////////////////////////////////////////////////////////////////////////////////////////
    if (*iterator=='\n'){ //If i write just '\n' as a command.
        continue;
    }

    while((*iterator)!='\n'){ //This while is used in order to find the newline symbol and to get rid of it basically.
        iterator++;
    }
    
    (*iterator)='\0'; 
    char * iterator2=iterator--;
    if((*iterator2)==' '){  //Even when you press spacebar in terminal when you type a command it deletes it thats all.
        (*iterator2='\0');
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if(input[0]!='^'&& !isdigit(input[0])&&input[0]!='\0'){    //When i put as a command only the symnol ^ then dont put it in history !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        Insertion_to_history(history,input); //Put commands in the history 
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    if (isdigit(input[0])){    //If we type a number to terminal (then we get the command from history and it will be executed afterwards)
        char * the_return;
        int number=atoi(input);
        the_return=Get_history_command(history,number);
        if (the_return==NULL){
            continue;
        }
        else {
            strcpy(input,the_return);
        }


    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////
    char * returning;                //If we write an alias in order to execute it 
    returning=Check_alias_command(an_alias,input);
    if(returning!=NULL){
        strcpy(input,returning);
    }

    ////////////////////////////////////////////////////////////////////////////////////////////////////

    //Depending on the result every time we continue to call Instruction_handler or not
    pid_t p;
    result=DEFAULT; 
    while (result!=THE_END&&result!=AM&&result!=HISTORY&&result!=ALIAS){

        result=Instruction_handler(input,history,an_alias,0,NULL,&p); //This function will handle the command.
        
    }

    if(result==THE_END){
            waitpid(p,NULL,0);
    }
    
   //////////////////////////////////////////////////////////////////////////////////////////////////////
   
}


return 0;
}
