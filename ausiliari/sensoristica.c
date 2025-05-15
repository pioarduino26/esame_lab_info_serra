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
    printf("Pianta strappata con successo.\n");

    // Reimpianto una nuova pianta della stessa specie
    serra->pianta = specie;
    printf("Reimpiantata una nuova pianta di specie '%s'.\n", serra->pianta.nome);
}
void leggiSensori(SerraDati *dati) {
    time_t t = time(NULL);
    dati->orario = *localtime(&t);
    dati->temperatura = rand() % 30 + 15; // 15-45 C
    dati->umidita = rand() % 40 + 30;
    dati->luce = rand() % 512;
    dati->umidita_terreno = rand() % (MAX_VALUE - MIN_VALUE) + MIN_VALUE;
    dati->livello_acqua = rand() % 500;
}

const char* determinaStagione(int mese) {
    if (mese >= 3 && mese <= 5) return "Primavera";
    if (mese >= 6 && mese <= 8) return "Estate";
    if (mese >= 9 && mese <= 11) return "Autunno";
    return "Inverno";
}
void controllaIrrigazione(int umidita_terreno, Pianta pianta, struct tm orario) {
    const char* stagione = determinaStagione(orario.tm_mon + 1);
    int soglia = pianta.umidita_min;

    if (strcmp(stagione, "Estate") == 0) {
        soglia += 50; // In estate, il suolo perde umidit  pi  rapidamente
    } else if (strcmp(stagione, "Inverno") == 0) {
        soglia -= 30; // In inverno, il suolo trattiene pi  umidit 
    }

    if (umidita_terreno >= soglia && umidita_terreno <= pianta.umidita_max) {
        printf("[%s] Motore acqua: SPENTO\n", pianta.nome);
    } else {
        printf("[%s] Motore acqua: ACCESO (Stagione: %s)\n", pianta.nome, stagione);
    }
}

void controllaVentolaRaffreddamento(int temperatura, struct tm orario) {
    int velocita;
    if (orario.tm_hour >= 6 && orario.tm_hour <= 18) {
        // Durante il giorno, la ventola gira pi  velocemente
        velocita = (temperatura - 15) * 12;
    } else {
        // Durante la notte, la ventola rallenta
        velocita = (temperatura - 15) * 6;
    }

    if (velocita > 255) velocita = 255;
    if (velocita < 50) velocita = 20;

    printf("Ventola raffreddamento: %d (Orario: %02d:%02d)\n", velocita, orario.tm_hour, orario.tm_min);
}
void controllaVentolaRiciclo(struct tm orario) {
    int velocita = (orario.tm_hour - 1) * 10 + 40;
    if (velocita < 40) velocita = 40;
    if (velocita > 255) velocita = 255;
    printf("Ventola riciclo aria: %d\n", velocita);
}