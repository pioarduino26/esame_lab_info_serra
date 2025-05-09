// in questo file sono contenute le varie funzioni del programma
#include "sensoristica.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#define MIN_VALUE 724
#define MAX_VALUE 1000
void selectionSort(SerraDati serre[], int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (serre[j].umidita_terreno < serre[min_idx].umidita_terreno) {
                min_idx = j;
            }
        }
        // Scambio
        SerraDati temp = serre[i];
        serre[i] = serre[min_idx];
        serre[min_idx] = temp;
    }
}
void irrigazioneInsetticida(SerraDati serre[], int *n) {
    char risposta[4];

    printf("\nAttenzione: alcune piante potrebbero essere sensibili all'insetticida.\n");
    printf("Vuoi applicarlo a tutte le serre? (si/no): ");
    scanf("%3s", risposta);

    if (strcmp(risposta, "si") == 0) {
        printf("\nAvvio del trattamento con insetticida su tutte le serre...\n");
        sleep(1);
        for (int i = 0; i < *n; i++) {
                 if (serre[i].pianta.resistenza_insetticida == 0) {
                    strappaPiantaESostituisci(&serre[i]);
                 }

             else {
                printf("La pianta '%s' e' resistente. Nessuna sostituzione effettuata.\n", serre[i].pianta.nome);
            }
            printf("Trattamento insetticida applicato alla serra %d (%s).\n", i + 1, serre[i].pianta.nome);
        }
        printf("Trattamento completato.\n");

    } else {
        printf("\nHai deciso di applicare l'insetticida solo su una serra.\n");
        printf("Premi INVIO per visualizzare l'elenco delle serre e scegliere quella desiderata...\n");

        while (getchar() != '\n');
        getchar();

        printf("\nElenco delle serre:\n");
        for (int i = 0; i < *n; i++) {
            printf("  %d) %s\n", i + 1, serre[i].pianta.nome);
        }
        printf("\nInserisci il numero della serra scelta: ");
        int scelta;
        scanf("%d", &scelta);

        if (scelta >= 1 && scelta <= *n) {
            if (serre[scelta - 1].pianta.resistenza_insetticida == 0) {
                strappaPiantaESostituisci(&serre[scelta - 1]);
            } else {
                printf("La pianta '%s' ha resistito. Nessuna sostituzione necessaria.\n", serre[scelta - 1].pianta.nome);
            }
            printf("Trattamento insetticida applicato alla serra %d (%s).\n", scelta, serre[scelta - 1].pianta.nome);
            printf("Trattamento completato.\n");
        } else {
            printf("Scelta non valida. Nessun trattamento eseguito.\n");
        }
    }
}
// funzione per strappare e sostituire piante
void strappaPiantaESostituisci(SerraDati *serra) {
    // Salvo i dati originali della specie
    Pianta specie = serra->pianta;

    // Simulo l’operazione meccanica di “strappare” la pianta
    printf("Strappo la pianta '%s' che non ha resistito all'insetticida...\n", specie.nome);
    // qui potresti aggiungere il codice di attivazione dello strappa-piante

    printf("Pianta strappata con successo.\n");

    // Reimpianto una nuova pianta della stessa specie
    serra->pianta = specie;
    printf("Reimpiantata una nuova pianta di specie '%s'.\n", serra->pianta.nome);
}
