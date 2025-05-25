#include "sensoristica.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#define MIN_VALUE 724
#define MAX_VALUE 1000


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




void leggiSensori(SerraDati *dati) {
    time_t t = time(NULL);
    dati->orario = *localtime(&t);
    dati->temperatura = rand() % 30 + 15; // 15-45�C
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

void salvaStoricoRec(SerraDati serre[], int n, int index, FILE* fp) {
    if (index >= n) {
        // caso base: tutte le serre processate
        return;
    }
    // Legge sensori per la serra corrente
    leggiSensori(&serre[index]);

    // Scrive su file i dati aggiornati
    fprintf(fp, "Serra %d: %s\n", index + 1, serre[index].pianta.nome);
    fprintf(fp, "  Temperatura: %d°C\n", serre[index].temperatura);
    fprintf(fp, "  Umidita': %d%%\n", serre[index].umidita);
    fprintf(fp, "  Luce: %d\n", serre[index].luce);
    fprintf(fp, "  Umidita' terreno: %d\n", serre[index].umidita_terreno);
    fprintf(fp, "  Livello acqua: %d\n", serre[index].livello_acqua);
    fprintf(fp, "  Orario: %02d:%02d\n", serre[index].orario.tm_hour, serre[index].orario.tm_min);
    fprintf(fp, "  Stagione: %s\n", determinaStagione(serre[index].orario.tm_mon + 1));
    fprintf(fp, "-----------------------------\n");

    // Chiamata ricorsiva per la prossima serra
    salvaStoricoRec(serre, n, index + 1, fp);
}




