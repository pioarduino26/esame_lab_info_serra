#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "ausiliari/sensoristica.h"
#include <string.h>
// Funzione per stampare lo stato delle serre in base alla tipologia di pianta
void stampaSerre(SerraDati serre[], int n) {
    printf("\nStato delle serre:\n");
    for (int i = 0; i < n; i++) {
        printf("Serra %d: %s - Umidita' terreno: %d\n", i + 1, serre[i].pianta.nome, serre[i].umidita_terreno);
    }
    printf("\n");
}
// Stampa informazioni sulla serra: indice, nome della pianta e umidità del terreno
void salvaLog(SerraDati serre[], int n) {
    printf("Salva il log delle attivita' delle serre.\n");
    printf("Inserisci il nome del file in cui vuoi salvare il log: ");
    char nomeFile[100];
    scanf("%99s", nomeFile);

    FILE* fileSalvataggio = fopen(nomeFile, "w");// Apertura del file in scrittura
    if (!fileSalvataggio) {
        // Messaggio di errore se il file non può essere creato/aperto
        printf("Errore: Impossibile creare il file di salvataggio.\n");
        return;
    }
