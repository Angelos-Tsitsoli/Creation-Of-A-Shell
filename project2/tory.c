#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h> 
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "structures.h"
#include <signal.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <glob.h>
#include <errno.h>

void catchinterrupt ( int signo ) {
printf ( "  nCatching : signo =% d \n " , signo ) ;
printf ( " Catching : returning \n " ) ;
    }

int main () {
static struct sigaction act ;
act . sa_handler = catchinterrupt ;
sigfillset (&( act . sa_mask ) ) ;
sigaction ( SIGINT , & act , NULL ) ;
printf ( " sleep call #1\n " ) ;
sleep(1) ;
printf ( " sleep call #2\n " ) ;
sleep(1) ;
printf ( " sleep call #3\n " ) ;
sleep (1) ;
printf ( " sleep call #4\n " ) ;
sleep(1) ;
printf ( " Exiting \n " ) ;
exit(0);
}
