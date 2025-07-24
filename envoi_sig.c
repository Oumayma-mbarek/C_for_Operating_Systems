#include <sys/types.h>
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

//Ce programme emet le signal dont on lui passe le numero 
//en premier argument
//vers le(s) processus identifie(s) par le second argument 


int main(int argc, char* argv[]){
    pid_t pid;
    int sig;
    if(argc<3){
        printf("%s:envoi_sig Numsig Dest/n",argv[0]);
        return 1;
    }
    sig=atoi(argv[1]);
    pid=atoi(argv[2]);
    if (kill(pid,sig)==-1){
        perror("kill");
        return errno;
    }
    return 0;
}

/* ./dormeur &
[1] 3156
./envoi_sig 1 3156
[1]+  Hangup  
*/ 
//on a envoye le signal 1 (hanup) au processus ./dormeur 
//ayant un pid de 3156