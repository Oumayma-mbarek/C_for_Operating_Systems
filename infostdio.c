#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

//ce programme renvoie le mode d'ouverture de chacun
//des trois entrees/sorties standard 

int main(int argc, char*argv[]){
    int fd,mode,cpt;
    int flags;
    //on traite les descripteurs 0,1,2
    for(cpt=0;cpt<3;cpt++){
        if (cpt==0){
            fd=STDIN_FILENO;
        }
        else{
            if (cpt==1){
                fd=STDOUT_FILENO;
            }
            else{
                fd=STDERR_FILENO;
            }
        }
        flags=fcntl(cpt,F_GETFL);
        if (flags==-1){
            perror("fcntl");
            return errno;
        }
        printf ("options d'ouverture de %d : %d",cpt,flags);
        mode=flags & O_ACCMODE;
        if (mode==O_RDONLY){
            printf("lecture\n");

        }
        else if (mode== O_WRONLY){
            printf("ecriture\n");
        }
        else if(mode==O_RDWR){
            printf("lecture/ecriture\n");
        }
        else {
            printf("inconnu");
        }

    }
}