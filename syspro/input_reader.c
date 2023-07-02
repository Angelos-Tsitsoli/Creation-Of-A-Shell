//#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "interface.h"
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <glob.h>
#include <signal.h>
static int last_position=0;   //This will basically hold the number of characters that where read.
static int ampersand=0;   //This is a flag to tell if we have a background symbol or not 

///////////////////////////////////////////////////////////////////////////////////////////
static struct sigaction in,qu; //After the call of the signal_avoidance function these structs will hold the default action of meeting the signals stop and interrupt , in order to restore them at signal_access after we change the way we handle them in signal_avoidance. 

int signal_access(void){ //Basically this function is used in order to restore the specific action every time to the old value they had before the call of the signal _avoidance
                              //Here the first argument will be the signal in which the action will be set.The second argument will be a pointer to an object that specifies the new action for the signal that was given . The third argument takes an action that specfies the old action for the signal 
   sigaction(SIGINT,&in,NULL); //the above will happen for the CTRL C for the interrupt signal. 
   sigaction(SIGTSTP,&qu,NULL); //the above will happen for the CTRL Z signal , stop signal. 
   return 0;

}
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////
int signal_avoidance(){  //This function basically is used in order to ignore the signals stop and interrupt
   static int tem=1;   //This is going to be used in order to know if the function is called before .
   struct sigaction avoid; //Through this struct we will set the action of ignoring the signal .

   memset(&avoid,0,sizeof(avoid)); //Initializing the struct.
   avoid.sa_handler=SIG_IGN;//Setting the sa.handler to ignore the signal
   avoid.sa_flags=SA_RESTART;
   if(tem==1){             //If tem is 1 then it means it's the first time we called this function so do the following . So we dont kn;ow the old action , thats why at the first call of the function we store the old actions to the in and qu structs.
      sigaction(SIGINT,&avoid,&in); //We set us the action to ignore the signal in both cases .The old signal handlers are stored to the in and qu .
      sigaction(SIGTSTP,&avoid,&qu);
      tem++; 
   }
   if(tem!=1){ //If the function is called more than once then we have already stored the old action that will be done (at in and qu structs)if a signal like these appears so we dont have to give a third argument.
      sigaction(SIGINT,&avoid,NULL);
      sigaction(SIGTSTP,&avoid,NULL);
   }
   return 0;
}
/////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////

void Clean(){  ///This basically restarts the number of characters that have been read
last_position=0;
}

/////////////////////////////////////////////////////////////////////////////// Through the following function we store a word character by character every time . The whole word will be stored eventually to the first parameter.

int Expression_store(char *word, char character)
{
   int i = 0;
   while (word[i] != '\0')
   {
      i++;
   }
   word[i] = character;
   i++;
   word[i] = '\0';
   
   return 0;
}

/////////////////////////////////////////////////////////////////////////////////

///////////////////////////Every time a command is executed we call this function to clear the characters of word in order to write another command later on
void Reseting(char * word){   
   // Reset iterator to beginning of word array
    char* iterator3 = word;

    // Loop through word array to count length of string
    int length = 0;
    while (*iterator3 != '\0') {
        length++;
        iterator3++;
    }

    // Clear contents of word array
    for (int i = 0; i < length; i++) {
        word[i] = '\0';
    }

}

//////////////////////////////////////////////////////////////////////////////////////////Basically this function is the parser and it recognizes every token ///////

enum argument Reading(char *term,struct History *history,char *word,struct Alias * a) 
{
   char *term_ptr; //Iterator that will go through every character.
   char * term_ptr2; //Iterator that will go through every character and will be used in case other symbol '>' is found.
   char* term_iterate; //Iterator that will go through every character and will be used in case we have a TERM.
   term_ptr = term; 
   enum condition con=DEFAULT;
   for (int i=0;i<last_position;i++){  //This will give the position in which we finished reading the command the last time , so we do that in order to continue now from there .
      *term_ptr++;
   }
   while ((*term_ptr) != '\0')  //While we haven't finished reading.
   {
      switch (con) //We check the condition every time .
      {
      case DEFAULT:  
         switch(*term_ptr) {  
         case '>':  //If the symbol is '>' then we check if the next symbol is '>' so we have a double '>>' .
            term_ptr2=term_ptr;
            term_ptr2++;
            if(*term_ptr2=='>'){  //If the next symbol is '>' then we add 2 to the iterator in order to get the next symbol of the second '>' in the next reading .
               con=DEFAULT;
               term_ptr=term_ptr+2; 
               last_position=last_position +2;
               return DOUBLE_RIGHT_ARROW;
            }
            if(*term_ptr2!='>'){  //If the next symbol is not'>' then this means we only have the symbol '>'.
               con=DEFAULT;
               term_ptr++;
               last_position++;
               return RIGHT_ARROW;
            }
            
         case '"':   //We change the con for the reading of the next symbol 
            con=QUOTATION_WORDS;
            term_ptr++;
            continue;

         case ';':  //We return SEM 
            con=DEFAULT;
            term_ptr++;
            last_position++;
            return SEM;

         case '<':  //We return LEFT_ARROW 
            con=DEFAULT;
            term_ptr++;
            last_position++;
            return LEFT_ARROW;

         case '|':   //We return VERT_BAR
            con=DEFAULT;
            term_ptr++;
            last_position++;
            return VERT_BAR;

         case '&':  //We return AM
            con=DEFAULT;
            term_ptr++;
            last_position++;
            return AM;

         case '^': //We return HISTORY 
            Print_history(history);
            return HISTORY;

         case '@':  //We return ALIAS
            Print_alias(a);
            return ALIAS;

         case ' ':    //We just ignore it 
            con=DEFAULT;
            term_ptr++;
            last_position++;
            continue;

         default:   //If none of the above happens then we end up here . We give to con the WORD_READING for the reading of the next character.
            con=WORD_READING;
            continue;

         }
      
      case WORD_READING:  //We know we are reading a a word so if we find any of the below symbols then just return.
         switch(*term_ptr) {
         case '>':    
            return TERM;
         case ';':
            return TERM;
         case '<':
            return TERM;
         case '|':
            return TERM;
         case '&':
            return TERM;
         case ' ':
            return TERM;
         case '\t':
            return TERM;

         default:   //Store the character every time 
            term_iterate=term_ptr;
            term_iterate++;
            Expression_store(word, (*term_ptr));
            if (*term_iterate=='\0'){
               last_position++;
               return TERM;  
            }
            term_ptr++;
            last_position++;
            continue;

         }
         
         case QUOTATION_WORDS: //If we are here then we know that we are reading inside quotes so just read and store untill you find the symbol '"' .Then return TERM
         switch(*term_ptr) {
            case '"':
               last_position++;
               con=TERM;
               return TERM;
            default:
               Expression_store(word,(*term_ptr));
              
               term_ptr++;
               last_position++;
               continue;
         }
      
      }
 
   }

   return THE_END; //When we finish just return THE_END.
} 
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////// This function here is responsible redirections wherever needed

void Redirection(char * input_file,char * output_file,enum argument in , enum argument out,int the_in,int the_out,int doub_right) {

if(the_in==0){ //If the command is going to be executed in the background and the input stream hasn't been changed we change it to the file /dev/null . We redirect to this file because we don't care about the output.
   if(ampersand==1){
      strcpy(input_file,"/dev/null");
      ampersand=0;
   }
   the_in=-1;
}

if (input_file[0]!='\0'){  //If the input_file is not null , which means it has a name of the file that the redirection of the input will be . Then the open function will be called in order to have the ability to opne the file with that name only to read the file  . 
   the_in=open(input_file,O_RDONLY,0);
}

dup2(the_in, STDIN_FILENO);  //The dup2 function will redirect the input from STDIN_FILENO to the file with file_descriptor the_in .
if(the_in!=STDIN_FILENO){  //If the dup2 changed then 
   close(the_in);      //Close the original file descriptor
}


if(output_file[0]!='\0'){  //If the output_file is not null , which means it has a name of the file that the redirection of the output will be . Then the open function will be called in order to have the ability to opne the file with that name  . 
   if(doub_right==1){  //If the variable doub_right equals to 1 then this means that the file that the output will be redirected must give the ability to append in it and not erase whatever it already has.
      the_out=open(output_file,  O_WRONLY | O_APPEND | O_CREAT, 0666); //So we open it with the flags for writing , appending and creating if it doesn't exist.
      if (the_out == -1) {  //For an error
        printf("An error occured opening the file\n");
        return ;
      } 
   }
   
   else if(doub_right==0){ //If the doub_right equals to zero then the file that the redirection will happen must give the ability to erase whatever it already has and write on top whatever we write.
      the_out=open(output_file,  O_WRONLY | O_TRUNC | O_CREAT, 0666); 
      if (the_out == -1) {
        printf("An error occured opening the file\n");
        return ;
      }  
   } 
}

dup2(the_out, STDOUT_FILENO);  //With dup2 redirect stdout to file and also dup2 duplicates the existing file
if(the_out!=STDOUT_FILENO){
   close(the_out); //close the original file descriptor.
}


return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////This function is used when a cd command is found.

void Cd_alternative(char * command){  
int result;
result=chdir(command);  //We get inside the folder we want to through this.
if(result<0){
   printf("Sorry error occured\n");
}
else { //This will just print the path of the folder we are in 
char buf[1024]; 
printf("%s\n",getcwd(buf, sizeof(buf)));
}
return ;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////// This function is used for checking if the other term that follows when reading is a TERM , because of the redirection that is going to happen if the next tern that follows is not a word then there's an error 
int Redir_check(char *input,struct History *history,char *the_input,struct Alias * A,int argc,char symbol){

enum argument ret;

ret=Reading(input,history,the_input,A); //We check if the next part of the command is TERM because if it isn't then we return an ERROR
if(ret!=TERM){
   if(symbol=='>'){
      printf("Not a word after the symbol > ");
   }
   if(symbol=='<'){
      printf("Not a word after the symbol < ");
   }
   return -5; 
}


return 0;
}
///////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////// This functions just checks if there's an alias
int Check_for_alias_commands(char * argv[],struct Alias *A){  

if (strcmp(argv[0],"createalias")==0){
   Insertion_to_alias_structure(A, argv[1],argv[2]);  //Insert the alias
   return 0;
}


if(strcmp("destroyalias",argv[0])==0){
   Destroy_an_alias(A,argv[1]); //Destroy the alias
   return 0;
                
}

return 1;
}
////////////////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////// This function check if we have a cd 

int Check_for_cd(char * argv[]){  //We check if the command has the cd in it .
  
if (strcmp(argv[0],"cd")==0){
   Cd_alternative(argv[1]);
   return 0;
}
return 1;
}
/////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////This function is responsible for executing the command (via execvp)
pid_t Execution (char * argv[],int argc,struct Alias *A,enum input in,enum output out ,char * the_in, char* the_out,int the_input,int the_output,int cl,int doub_right){
int ret_pid;//For the Check_for_cd
int ret_cd;
ret_cd=Check_for_cd(argv); //We are checking for the a cd command
if(ret_cd==0){  
   return ret_pid; 
}
int alias_check;
alias_check=Check_for_alias_commands(argv,A); //We are checking for a alias command
if (alias_check==0){
   return ret_pid;
}

ret_pid=fork();
    if (ret_pid==-1) {   //If there's an error 
        printf("error\n");
        exit(EXIT_FAILURE);
    }
    if (ret_pid == 0) {   //For the child process
        if(cl!=-1){  //Close the writing file , if the part of the command that sees it only needs the reading part of the pipe or else ignore it .
         close(cl);
        }

    if(ampersand==0){ //If we dont have a background command then let the signals
      signal_access();
    }
        
        Redirection(the_in,the_out,in,out,the_input,the_output,doub_right); //For the redirections
        argv[argc]=NULL;  
        execvp(argv[0],argv);  //Execute the command
        perror("execvp"); // if execvp fails
        exit(EXIT_FAILURE); 
    } else {
         if(ampersand==1){  //If we have a background command
            printf("%ld\n",(long)ret_pid);
            ampersand=0;
         }

        if(the_input>STDIN_FILENO){ //If the input is different from the STDIN_FILENO , in our program this could happen if the time we come here we are at the right part of a pipe and the command redirected the input in order to read from the pipe . So we need to close it from the parent process in order to continue , because if it is held open we could have a problem on finishing
            
            close(the_input);
        }

        if(the_output>STDOUT_FILENO){ //If the output is different from the STDOUT_FILENO , in our program this could happen if the time we come here we are at the left part of a pipe and the command redirected the output in order to write from the pipe . So we need to close it from the parent process at the right time in order to continue , because if it is held open we could have a problem on finishing
            close(the_output);
        }

       return ret_pid; //Every time we return the pid in order to use the waitpid function from the place where Instruction_handler is called
     }
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////// This function basically handles the situation of a wild card
int Filling_argv_and_wild_card_check(int argc,char ** argv,char * word){
glob_t glob_struct; 
int i;
int argc2=argc;                   
glob(word,GLOB_NOMAGIC,NULL,&glob_struct); //This will store the matching filenames (that are in the current directory and match the pattern that is specified in word )in a struct called glob_struct . The GLOB_NOMAGIC matches only the files with ames that they match exactly with the pattern that is specified in "wprd"
i=glob_struct.gl_pathc;  //So we give to the argv every possible match that happened by this loop/\.
for (int j=0; j<i;j++){
   argv[argc2] = /*(char *)*/malloc(100 * sizeof(char)); //Taking memory
   strcpy(argv[argc2],glob_struct.gl_pathv[j]);
   argc2++;
}

return argc2;
}
///////////////////////////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////This function is used for the creation of a pipe/////////
int Pipe_creation(int pipe_checking,int fds_for_pipe[] , int length , int * pipe_des ){

if(pipe_checking==1){ //If the pipe_checking is one then only then we can create the pipe 
   pipe(fds_for_pipe); //This function will create it and will give as an argument the array of 2 elements
   *pipe_des=fds_for_pipe[length-1]; //We give the pointer the descriptor for the writing part of the command in order to give it to the previous command of the pipe that needs to write in it 
   return fds_for_pipe[length-2];  // we return the descriptor for the reading part of the pipe so that the right part of the pipe can read from it .
}

else return 0; //else we just return zero which means the input hasn't changed it still remained the STDIN_FILENO

}
//////////////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////// This function examines every command and calls the above functions in order to execute them 
enum argument Instruction_handler(char*input,struct History * history,struct Alias *A,int pipe_checking , int*pipe_des , pid_t * child){
int fds_for_pipe[2];// This array will be used for the pipe the descriptor of writing and reading part
fds_for_pipe[0]=-1; // I give them as first values -1
fds_for_pipe[1]=-1;
char * argv[60]; //This will hold the parts of the command
int argc=0; //Iterator of the argv 
char word[100]; //Through this a part of the command will be given to the argv
char the_input[100]; //The name of the file that the redirection of the input will happen 
char the_output[100]; //The name of the file that the redirection of the output will happen 
word[0] = '\0'; //We initialize them with the symbol '\0'
the_input[0] = '\0';
the_output[0] = '\0';
enum input input_from;
enum output dest_to;
enum argument state; //This will hold the state every time for every part of the command and it will be handled accordingly
int in=STDIN_FILENO; //STDIN_FILENO
int out=STDOUT_FILENO; //STDOUT_FILENO
enum argument state2; //This will be returned at the cases VERT_BAR,THE_END,SEM.
int after_exec; //This will give the pid of the child that fork made to the point that the function Instruction_handler was called
int lt_gt_check; //This just checks if there's a problem with the symbols '>' or '<' or '>>'
int doub_right=0; //This will tell if its about the symbol '>>'

state=DEFAULT; //Every time we get the state to DEFAULT becuse the problem would be that the state took the enum argument THE_END and it wouldn't come through the while loop
   while(state!=THE_END&&state!=HISTORY&&state!=ALIAS){   //If the state id THE_END or HISTORY or ALIAS then stop the loop in order to return 
      state=Reading(input,history,word,A); //The parser will give everytime the kind of enum argument of the command to handle 
      switch (state){
          case TERM:  //If it's a word then check for wild cards and put it in the argv 
              argc=Filling_argv_and_wild_card_check(argc,argv,word);  //The new value of the argc will be returned and given to argc , it's basically argc + 1 every time in order to store the next part of the command to the next position of the argv  
              break; //And break , don't ceck the below 
              
          case AM: //This means that the command has the symbol '&' so we have to put 1 to the variable (the value 1 means that the command will be executed to the background)
              ampersand=1;
              break; //Don't check the below break and continue the while with the next part

          case SEM:   //If state has the value SEM,VERT_BAR or THE_END do the following
          case VERT_BAR:
          case THE_END:

              if (state==VERT_BAR){ // If state equals to the VERT_BAR then call the instruction_handler.
                  state2=Instruction_handler(input,history,A,1,&out,child); //The value 1 means that a pipe should be created .The out argument was given so when the function would return the writing part of the pipe would be hold to out , also child would store the pid of the child process that would be created from fork in Execution , so that it could be usedfor the waitpid.
              }
              
              if (state!=VERT_BAR){//If the case is not VERT_BAR then give to state2 the value of the state in order to return it .
                  state2=state;
              }
             
              in=Pipe_creation(pipe_checking,fds_for_pipe,2,pipe_des); //This function will check if it a pipe should be created  and  if it should , then create a pipe and give to the variable 'in' the file descriptor  of the reading part of the pipe , else just return the value that the variable 'in' already had.

              after_exec=Execution(argv,argc,A,input_from,dest_to,the_input,the_output,in,out,fds_for_pipe[1],doub_right); //The Execution function will be called in order to execute a command and it will return the value of the pid of the child process that was created in the Execution function , this would happen in order to use it for the waitpid as i referred before.

              if (state!=VERT_BAR){ //This happened so that the variable that he *child pointer pointed took a value when after the recursion happened  
                  *child=after_exec;
              }
              
              return state2;
         
          case LEFT_ARROW:// At this case we check the next part of the command if it is a TERM and continue , if there's a false then break
              lt_gt_check=Redir_check(input,history,the_input,A,argc,'<');
              if(lt_gt_check==-5){
                  break;
              }
              continue;

          case HISTORY: //In this case then just return the state
               state=HISTORY;
               return state;

          case ALIAS: //In this case then just return it
               state=ALIAS;
               return state;

          case RIGHT_ARROW:  //If we have redirection of the output with '>' or '>>' then check the next part of the command if it is a TERM as before , if there's a error then break.
          case DOUBLE_RIGHT_ARROW:
              dest_to=ANOTHER_OUTPUT;
              doub_right=1;
              lt_gt_check=Redir_check(input,history,the_output,A,argc,'>');
              if(lt_gt_check==-5){
                  break;
              }
              continue;
         
      }
        
    Reseting(word); //Every time erase what word has in order to store the next parts of the command
    
   }

   return ERROR; //If there wasn't a return from the above then there's an ERROR.

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
















