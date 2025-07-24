#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

#define CHK(op) do { if ((op) == -1) raler (1, #op); } while (0)

noreturn void raler (int syserr, const char *msg, ...)
{
    va_list ap;

    va_start (ap, msg);
    vfprintf (stderr, msg, ap);
    fprintf (stderr, "\n");
    va_end (ap);

    if (syserr == 1)
        perror ("");

    exit (EXIT_FAILURE);
}

/** fonction qui sépare la chaine de caractères 'chaine'
 *  avec les séparateurs ':'
 *  renvoie un tableau NULL-terminated de mots
 *  utilise la mémoire de chaine pour cela (modifie la chaine) !
 */
char **separe (char *chaine)
{
    // on compte le nombre de répertoire contenu dans 'chaine'
    int i = 0, rep = 1;
    while (chaine [i] != '\0')
        if (chaine [i++] == ':')
            rep++;

    // création du tableau qui contiendra les noms des répertoires
    char **tab_rep;
    if ((tab_rep = malloc ((rep + 1) * sizeof *tab_rep)) == NULL)
        raler (1, "malloc");
    tab_rep [rep] = NULL;

    i = 0;
    for (int j = 0 ; j < rep ; ) {
        if (chaine [i] == ':' || chaine [i] == '\0') { // on a le nom complet d'un répertoire
            chaine [i] = '\0';

            if ((tab_rep [j] = malloc (strlen (chaine) + 1)) == NULL)
                raler (1, "malloc tab_rep");
            strcpy (tab_rep [j], chaine);

            chaine = &chaine [i] + 1;                  // on passe au répertoire suivant
            i = 0;
            j++;
        }
        else
            i++;
    }

    return tab_rep;
}



int main ()
{
    int i ;
    char *ge_path, *path, **tab ;

    // récupère la valeur de $PATH
    ge_path = getenv ("PATH") ;
    if (ge_path == NULL)
	raler (0, "cannot find PATH variable") ;
    // crée une copie locale modifiable
    // (on ne doit pas modifier la zone mémoire renvoyée par getenv)
    path = malloc (strlen (ge_path) + 1) ;
    if (path == NULL)
	raler (1, "cannot malloc string for %s", ge_path) ;

    strcpy (path, ge_path) ;

    tab = separe (path) ;

    for (i = 0 ; tab [i] != NULL ; i++)
    {
        printf ("%s\n", tab [i]) ;
    }

    free (tab) ;
    free (path) ;

    exit (0) ;
}