// ce programme duplique la sortie standard sur la sortie d'erreur standard
//il ecrit ensuite sur la sortie d'erreur standard, ecrivant en fait
//sur la sortie standard


#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]){
    //on ferme la sortie d'erreur standard
    close(STDERR_FILENO);
    //on duplique le descripteur de la sortie standard
    //comme la sortie d'erreur est fermee , c'est son descripteur 
    //qui sera utilise pour la duplication(le plus petit disponible) 
    int fddup;
    fddup=dup(STDOUT_FILENO);
    if (fddup==-1){
        perror("dup");
        return errno;
    }
    //on controle que c'est bien la sortie d'erreur standard
    if (fddup != STDERR_FILENO){
        perror(" mauvaise duplication");
        return errno;
    }
    else{
        //on ecrit vers la sortie d'erreur standard
        //redirigee vers la sortie standard 
        write(fddup,"Ecrit sur STDERR\n",18);
    }
    return 0;

}