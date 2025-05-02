#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ausiliari/sensoristica.h"
#include <string.h>
//stampo le serre in base alla tipologia di piante
void stampaSerre(SerraDati serre[], int n) {
    printf("\nStato delle serre:\n");
    for (int i = 0; i < n; i++) {
        printf("Serra %d: %s - Umidita' terreno: %d\n", i + 1, serre[i].pianta.nome, serre[i].umidita_terreno);
    }
    printf("\n");
}
// creo il file per il salvataggio dei dati serra
void salvaLog(SerraDati serre[], int n) {
    printf("Salva il log delle attivita' delle serre.\n");
    printf("Inserisci il nome del file in cui vuoi salvare il log: ");
    char nomeFile[100];
    scanf("%99s", nomeFile);

    FILE* fileSalvataggio = fopen(nomeFile, "w");
    if (!fileSalvataggio) {
        printf("Errore: Impossibile creare il file di salvataggio.\n");
        return;
    }
