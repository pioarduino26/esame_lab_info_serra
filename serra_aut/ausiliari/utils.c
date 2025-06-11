#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

// Funzione per eliminare tutti i file .txt da una directory specificata
void eliminaFileTXT(const char* dir_path) {
    DIR *dir;
    struct dirent *entry;

    // Apro la directory specificata
    dir = opendir(dir_path);
    if (dir == NULL) {
        printf("Errore nell'apertura della directory: %s\n", dir_path);
        perror("Errore: "); // Stampa il motivo dell'errore ad esempio: "file non trovato"
        return;
    }

    // Scansione dei file nella directory
    while ((entry = readdir(dir)) != NULL) {
        // Controllo se il file ha estensione .txt
        if (strlen(entry->d_name) > 4 && strcmp(entry->d_name + strlen(entry->d_name) - 4, ".txt") == 0) {
            // Crea il percorso completo del file
            char file_path[1024];
            snprintf(file_path, sizeof(file_path), "%s/%s", dir_path, entry->d_name);

            // Elimina il file
            if (remove(file_path) == 0) {
                printf("File eliminato: %s\n", file_path);
            } else {
                printf("Errore nell'eliminazione del file: %s\n", file_path);
                perror("Errore: "); // Stampa il motivo dell'errore
            }
        }
    }

    // Chiude la directory
    if (closedir(dir) != 0) {
        perror("Errore nella chiusura della directory: ");
    }
}
