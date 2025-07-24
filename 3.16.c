#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Fonction pour séparer une chaîne en un tableau de chaînes, en utilisant ':' comme séparateur
char** split_path(const char* path) {
    char *ptr = strtok(strdup(path), ":");
    char **array = NULL;
    int count = 0;

    while (ptr != NULL) {
        array = realloc(array, (count + 1) * sizeof(char*));
        array[count++] = ptr;
        ptr = strtok(NULL, ":");
        count ++;
    }

    return array;
}

int main() {
    char *path = getenv("PATH");
    if (path == NULL) {
        perror("getenv");
        return 1;
    }

    char **paths = split_path(path);

    for (int i = 0; paths[i] != NULL; i++) {

        printf("%s\n", paths[i]);
    }

    // Libérer la mémoire allouée
    for (int i = 0; paths[i] != NULL; i++) {
        free(paths[i]);
    }
    free(paths);

    return 0;
}