#include <unistd.h>
//Ce programme boucle indefiniment 
//il affiche un message, attend 10 secondes et recommence 

int main(void){
    while (1){
        write(STDOUT_FILENO,"je dors\n",8);
        sleep(10);
    }
}

// ./dormeur & execute le fichier dans le background 
//je peux voir le resultat dans le terminal mais j'ai controle 
// du terminal 
//When a command runs in the background, the shell assigns it
// a job number (e.g., [1]) and a PID