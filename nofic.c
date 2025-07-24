//ce programme essaye de supprimer un fichier inexistant
//et affiche le message d'erreur standard 

#include <stdio.h>
#include <string.h>
#include <unistd.h>
int main (void){
    int res= unlink ("NoFile");
    if(res){
        perror("Erreur Suppression ");
    }
}