#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10

struct mois{
    char nom[9+1];  //nom du mois
    int jours;  //nombre de jours du mois
};

int main(){

    const struct mois annee[12] = {
        {"janvier", 31},
        {"fevrier", 28}, 
        {"mars", 31},
        {"avril", 30},
        {"mai", 31},
        {"juin", 30},
        {"juillet", 31},
        {"août", 31},
        {"septembre", 30},
        {"octobre", 31},
        {"novembre", 30},
        {"decembre", 31}
    };

    char a[SIZE];
    int found =1;
    scanf("%s",a);
    for(int i=0;i<12;i++){
        if(strcmp(a,annee[i].nom)== 0){
            printf("le mois numero: %d contient %d jours\n", i+1, annee[i].jours);
            found=0;
        }     
    }
    if(found==1){
        printf("mois invalide\n");
    }
    return 0;
}