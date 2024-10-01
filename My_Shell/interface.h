typedef struct Node_of_linked_list * Node_ptr;

struct Node_of_linked_list {   
           char  Command[300]; 
           int position;
           Node_ptr Nextcommand; 
};

struct History{    
     Node_ptr Front;
     Node_ptr Rear; 
     int Number_of_commands;
};

enum condition{
   DEFAULT,
   WORD_READING,
   QUOTATION_WORDS,
};


enum argument{
   TERM,
   SEM,
   AM,
   VERT_BAR,
   LEFT_ARROW,
   RIGHT_ARROW,
   DOUBLE_RIGHT_ARROW,
   THE_END,
   HISTORY,
   ALIAS,
   ERROR
   
};

enum input{
   DEFAULT_INPUT,
   ANOTHER_INPUT
};

enum output{
   DEFAULT_OUTPUT,
   ANOTHER_OUTPUT
};



typedef struct Alias_node * Alias_node_ptr;

struct Alias_node {   
           char  original_command[300]; 
           char  alias_command[300];
           int position;
           Alias_node_ptr Nextcommand; 
};

struct Alias{    
     Alias_node_ptr Front;
     Alias_node_ptr Rear; 
};


char * Get_history_command(struct History *,int );
enum argument Reading (char * , struct History *,char *,struct Alias * );
enum argument Instruction_handler (char*,struct History * ,struct Alias *,int,int *,pid_t *);
void Initialize_history(struct History **);
void Insertion_to_history(struct History *, char *);
void Print_history(struct History *);
void Print_alias(struct Alias *);
void Free_history(struct History *);
void Free_alias(struct Alias * );
void Initialize_alias_structure(struct Alias **);
char * Check_alias_command(struct Alias *,char *);
void Initialize_history(struct History **);
void Insertion_to_history(struct History *, char *);
char * Get_history_command(struct History *,int );
void Print_history(struct History *);
void Free_history(struct History * );
void Insertion_to_alias_structure(struct Alias *, char *,char * );
char * Check_alias_command(struct Alias *,char * );
void Destroy_an_alias(struct Alias *,char * );
void Print_alias(struct Alias *);
void Free_alias(struct Alias * );
void Clean();
int signal_avoidance();